/* NOTE: Do not edit this file, it is generated by a script:
   Export.py --api gl 4.2 --api wgl 4.0 --api glx 4.0 --api cgl 1.4 --api egl 1.0 --outdir src/regal
*/

/*
  Copyright (c) 2011 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
  OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalConfig.h"
#include "RegalContext.h"
#include "RegalDebugInfo.h"
#include "RegalContextInfo.h"

#include "RegalMarker.h"
#if REGAL_EMULATION
#include "RegalObj.h"
#include "RegalPpa.h"
#include "RegalBin.h"
#include "RegalDsa.h"
#include "RegalIff.h"
#include "RegalVao.h"
#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using namespace Logging;

RegalContext::RegalContext()
: dispatcher(),
  dbg(NULL),
  info(NULL),
  marker(NULL),
#if REGAL_EMULATION
  emuLevel(0),
  obj(NULL),
  ppa(NULL),
  bin(NULL),
  dsa(NULL),
  iff(NULL),
  vao(NULL),
#endif
#if defined(__native_client__)
  naclES2(NULL),
  naclResource(NULL),
#endif
  sysCtx(NULL),
  thread(0),
  logCallback(NULL),
  depthBeginEnd(0),
  depthPushAttrib(0)
{
  Internal("RegalContext::RegalContext","()");
  if (Config::enableDebug) {
    dbg = new DebugInfo();
    dbg->Init(this);
  }
}

void
RegalContext::Init()
{
  Internal("RegalContext::Init","()");

  info = new ContextInfo();
  RegalAssert(this);
  RegalAssert(info);
  info->init(*this);

  marker = new Marker;

#if REGAL_EMULATION
#if !REGAL_FORCE_EMULATION
  if
  (
    Config::forceEmulation  ||
    (
      Config::enableEmulation &&
      (
        info->core ||
        info->gles ||
        ( info->compat && !info->gl_ext_direct_state_access )
      )
    )
  )
#endif
  {
    RegalAssert(info);
    // emu
    emuLevel = 6;
    #if REGAL_EMU_VAO
    if (Config::enableEmuVao)
    {
      vao = new RegalVao;
      vao->emuLevel = 1;
      vao->Init( this );
    }
    #endif /* REGAL_EMU_VAO */
    #if REGAL_EMU_IFF
    if (Config::enableEmuIff)
    {
      iff = new RegalIff;
      iff->emuLevel = 2;
      iff->Init( this );
    }
    #endif /* REGAL_EMU_IFF */
    #if REGAL_EMU_DSA
    if (Config::enableEmuDsa)
    {
      Internal("RegalContext::Init","GL_EXT_direct_state_access");
      info->regal_ext_direct_state_access = true;
      info->regalExtensionsSet.insert("GL_EXT_direct_state_access");
      info->regalExtensions = ::boost::print::detail::join(info->regalExtensionsSet,std::string(" "));
      dsa = new RegalDsa;
      dsa->emuLevel = 3;
      dsa->Init( this );
    }
    #endif /* REGAL_EMU_DSA */
    #if REGAL_EMU_BIN
    if (Config::enableEmuBin)
    {
      bin = new RegalBin;
      bin->emuLevel = 4;
      bin->Init( this );
    }
    #endif /* REGAL_EMU_BIN */
    #if REGAL_EMU_PPA
    if (Config::enableEmuPpa)
    {
      ppa = new RegalPpa;
      ppa->emuLevel = 5;
      ppa->Init( this );
    }
    #endif /* REGAL_EMU_PPA */
    #if REGAL_EMU_OBJ
    if (Config::enableEmuObj)
    {
      obj = new RegalObj;
      obj->emuLevel = 6;
      obj->Init( this );
    }
    #endif /* REGAL_EMU_OBJ */

  }
#endif
}

RegalContext::~RegalContext()
{
  Internal("RegalContext::~RegalContext","()");
  delete info;
  delete marker;

#if REGAL_EMULATION
  // emu
  delete obj;
  delete ppa;
  delete bin;
  delete dsa;
  delete iff;
  delete vao;
#endif
}

REGAL_NAMESPACE_END
