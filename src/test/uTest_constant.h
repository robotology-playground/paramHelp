#ifndef uTest_CONSTANTS_H
#define uTest_CONSTANTS_H

#include <paramHelp/paramProxyBasic.h>

using namespace paramHelp;

namespace uTest_ParamHelp
{

    static const double     uTest_DEFAULT_DOUBLE(5.0);
    static const int        uTest_DEFAULT_INT(3);
    static const bool       uTest_DEFAULT_BOOL(true);

    enum uTest_ParamHelp_ParamId {
        PARAM_ID_DOUBLE,
        PARAM_ID_INT,
        PARAM_ID_BOOL,
        // ******************************************************************************************* //
        PARAM_ID_SIZE
    };

    const ParamProxyInterface *const uTest_ParamDescr[PARAM_ID_SIZE] =
    {
        //                          NAME                ID               SIZE   CONSTRAINTS     I/O ACCESS        DEFAULT VALUE                                          DESCRIPTION
        new ParamProxyBasic<double>("param_double",     PARAM_ID_DOUBLE,    1,                  PARAM_IN_OUT,  &uTest_DEFAULT_DOUBLE,                                  "A double param"),
        new ParamProxyBasic<int>("param_int",           PARAM_ID_INT,       1,                  PARAM_IN_OUT,  &uTest_DEFAULT_INT,                                     "An int param"),
        new ParamProxyBasic<bool>("param_bool",     PARAM_ID_BOOL,          1,                  PARAM_IN_OUT,  &uTest_DEFAULT_BOOL,                                    "A bool param")
    };

}

#endif
