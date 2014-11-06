/**
********************************************************************************
\file   sdocom.c

\brief  SDO command layer wrapper

This file manages the available SDO stacks. The function calls are forwarded
to the SDO stack defined in the api init parameters.

\ingroup module_sdocom
*******************************************************************************/

/*------------------------------------------------------------------------------
Copyright (c) 2014, Bernecker+Rainer Industrie-Elektronik Ges.m.b.H. (B&R)
Copyright (c) 2013, SYSTEC electronic GmbH
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holders nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
------------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <user/sdocom.h>

//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------
tSdoComFunctions* sdocomdummy_getInterface(void);
tSdoComFunctions* sdocomstandard_getInterface(void);

//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static tSdoComFunctions* pSdoComInstance;

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

//------------------------------------------------------------------------------
/**
\brief  Initialize SDO stack

The function initializes the SDO stack.

\param stackType_p             variable that defines which SDO stack to use.

\return The function returns a tOplkError error code.

\ingroup module_sdocom
*/
//------------------------------------------------------------------------------
tOplkError sdocom_init(UINT stackType_p)
{
    tOplkError ret = kErrorOk;

    switch (stackType_p)
    {
       case tOplkApiTestSdoCom:
       case tOplkApiTestSdoSeq:
            pSdoComInstance = sdocomdummy_getInterface();
           break;

       default:
       case tOplkApiStdSdoStack:
            pSdoComInstance = sdocomstandard_getInterface();
           break;
    }

    ret = pSdoComInstance->pfnInit();

    return ret;
}

//------------------------------------------------------------------------------
/**
\brief  Initialize SDO layer instance

The function initializes the SDO layer

\return The function returns a tOplkError error code.

\ingroup module_sdocom
*/
//------------------------------------------------------------------------------
tOplkError sdocom_addInstance(void)
{
    tOplkError ret = kErrorOk;

    ret = pSdoComInstance->pfnAddInstance();

    return ret;
}

//------------------------------------------------------------------------------
/**
\brief  Delete SDO layer instance

The function deletes an instance of the SDO layer.

\return The function returns a tOplkError error code.

\ingroup module_sdocom
*/
//------------------------------------------------------------------------------
tOplkError sdocom_delInstance(void)
{
    tOplkError ret = kErrorOk;

    ret = pSdoComInstance->pfnDelInstance();

    return ret;
}

//------------------------------------------------------------------------------
/**
\brief  Initialize a SDO layer connection

The function initializes a SDO layer connection. It tries to reuse an
existing connection to the specified node.

\param  pSdoComConHdl_p         Pointer to store the layer connection
                                handle.
\param  targetNodeId_p          Node ID of the target to connect to.
\param  sdoType_p               Type of the SDO connection.

\return The function returns a tOplkError error code.

\ingroup module_sdocom
*/
//------------------------------------------------------------------------------
tOplkError sdocom_defineConnection(tSdoComConHdl* pSdoComConHdl_p, UINT targetNodeId_p,
                                   tSdoType sdoType_p)
{
    tOplkError ret = kErrorOk;

    ret = pSdoComInstance->pfnDefineCon(pSdoComConHdl_p, targetNodeId_p, sdoType_p);

    return ret;
}

//------------------------------------------------------------------------------
/**
\brief  Initialize a transfer by index command

The function initializes a "transfer by index" operation for a connection.

\param  pSdoComTransParam_p     Pointer to transfer command parameters

\return The function returns a tOplkError error code.

\ingroup module_sdocom
*/
//------------------------------------------------------------------------------
tOplkError sdocom_initTransferByIndex(tSdoComTransParamByIndex* pSdoComTransParam_p)
{
    tOplkError ret = kErrorOk;

    ret = pSdoComInstance->pfnTransByIdx(pSdoComTransParam_p);

    return ret;
}

//------------------------------------------------------------------------------
/**
\brief  Get remote node ID of connection

The function returns the node ID of the remote node of a connection.

\param  sdoComConHdl_p          Handle of connection.

\return The function returns the node ID of the remote node or C_ADR_INVALID
        on error.

\ingroup module_sdocom
*/
//------------------------------------------------------------------------------
UINT sdocom_getNodeId(tSdoComConHdl sdoComConHdl_p)
{
    UINT node;

    node = pSdoComInstance->pfnGetNodeId(sdoComConHdl_p);

    return node;
}

//------------------------------------------------------------------------------
/**
\brief  Get command layer connection state

The function returns the state of a command layer connection.

\param  sdoComConHdl_p          Handle of the command layer connection.
\param  pSdoComFinished_p       Pointer to store connection information.

\return The function returns a tOplkError error code.

\ingroup module_sdocom
*/
//------------------------------------------------------------------------------
tOplkError sdocom_getState(tSdoComConHdl sdoComConHdl_p, tSdoComFinished* pSdoComFinished_p)
{
    tOplkError ret = kErrorOk;

    ret = pSdoComInstance->pfnGetState(sdoComConHdl_p, pSdoComFinished_p);

    return ret;
}

//------------------------------------------------------------------------------
/**
\brief  Abort a SDO transfer

The function aborts an SDO transfer.

\param  sdoComConHdl_p          Handle of the connection to abort.
\param  abortCode_p             The abort code to use.

\return The function returns a tOplkError error code.

\ingroup module_sdocom
*/
//------------------------------------------------------------------------------
tOplkError sdocom_abortTransfer(tSdoComConHdl sdoComConHdl_p, UINT32 abortCode_p)
{
    tOplkError ret = kErrorOk;

    ret = pSdoComInstance->pfnSdoAbort(sdoComConHdl_p, abortCode_p);

    return ret;
}

//------------------------------------------------------------------------------
/**
\brief  Delete a layer connection

The function closes and deletes an existing layer connection.

\param  sdoComConHdl_p          Connection handle of command layer connection
                                to delete.

\return The function returns a tOplkError error code.

\ingroup module_sdocom
*/
//------------------------------------------------------------------------------
tOplkError sdocom_undefineConnection(tSdoComConHdl sdoComConHdl_p)
{
    tOplkError ret = kErrorOk;

    ret = pSdoComInstance->pfnDeleteCon(sdoComConHdl_p);

    return ret;
}