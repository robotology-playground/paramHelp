#include <yarp/os/all.h>
#include <paramHelp/paramHelperServer.h>
#include "uTest_constant.h"
#include <typeinfo>

class uTest_ParamHelp_module: public yarp::os::RFModule
{

    double t;
    double param_double;
    int param_int;
    bool param_bool;
    yarp::os::Port  rpcPort;		// a port to handle rpc messages
public:
    paramHelp::ParamHelperServer *paramHelper;

    uTest_ParamHelp_module():
        t(0.0),
        param_double(0.0),
        param_int(0),
        param_bool(true)
    {}

    double getPeriod(){return 0.1;}

    bool configure(yarp::os::ResourceFinder &rf)
    {
        std::cout<<"CONFIGURING..."<<std::endl;
        paramHelper = new ParamHelperServer(uTest_ParamHelp::uTest_ParamDescr, uTest_ParamHelp::PARAM_ID_SIZE,
                                            NULL, 0);
        paramHelper->linkParam(uTest_ParamHelp::PARAM_ID_DOUBLE, &param_double);
        paramHelper->linkParam(uTest_ParamHelp::PARAM_ID_INT, &param_int);
        paramHelper->linkParam(uTest_ParamHelp::PARAM_ID_BOOL, &param_bool);

        if(!paramHelper->init("paramHelper_uTest"))
        {
            fprintf(stderr, "Error while initializing parameter helper. Closing module.\n");
            return false;
        }
        rpcPort.open("/uTest_ParamHelper/rpc");
        setName("uTest_ParamHelper");
        attach(rpcPort);
        return true;
    }

    bool interruptModule()
    {
        rpcPort.interrupt();
        return true;
    }

    bool close()
    {
        if(paramHelper){    paramHelper->close();       delete paramHelper;     paramHelper = 0;    }
        rpcPort.close();
        return true;
    }

    bool updateModule()
    {
        t = t + getPeriod();
        if( t >= 5.0 )
        {
            std::cout<<"MODULE TEST IS RUNNING"<<std::endl;
            t = 0.0;
            std::cout<<"****************************"<<std::endl;
            std::cout<<"param_double: "<<param_double<<" is type "<<typeid(param_double).name()<<std::endl;
            std::cout<<"param_int: "<<param_int<<" is type "<<typeid(param_int).name()<<std::endl;
            std::cout<<"param_bool: "<<param_bool<<" is type "<<typeid(param_bool).name()<<std::endl;
            std::cout<<"****************************"<<std::endl;
        }


        return true;
    }

    bool respond(const yarp::os::Bottle& cmd, yarp::os::Bottle& reply)
    {
        paramHelper->lock();
        if(!paramHelper->processRpcCommand(cmd, reply))
            reply.addString( (std::string("Command ")+cmd.toString().c_str()+" not recognized.").c_str());
        paramHelper->unlock();

        // if reply is empty put something into it, otherwise the rpc communication gets stuck
        if(reply.size()==0)
            reply.addString( (std::string("Command ")+cmd.toString().c_str()+" received.").c_str());
        return true;
    }
};

int main(int argc, char* argv[])
{
    yarp::os::Network yarp;
    yarp::os::ResourceFinder rf;

    if (!yarp.checkNetwork())
    {
        fprintf(stderr,"Sorry YARP network is not available\n");
        return -1;
    }

    uTest_ParamHelp_module test;
    test.configure(rf);

    return test.runModule();
}
