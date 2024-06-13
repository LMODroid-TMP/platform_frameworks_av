/*
 * Copyright (C) 2022 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <aidl/android/hardware/audio/effect/BnEffect.h>
#include <audio_effects/effect_downmix.h>

#include "DownmixContext.h"
#include "effect-impl/EffectImpl.h"

namespace aidl::android::hardware::audio::effect {

class DownmixImpl final : public EffectImpl {
  public:
    static const std::string kEffectName;
    static const Descriptor kDescriptor;
    DownmixImpl() { LOG(DEBUG) << __func__; }
    ~DownmixImpl() {
        cleanUp();
        LOG(DEBUG) << __func__;
    }

    ndk::ScopedAStatus commandImpl(CommandId command) REQUIRES(mImplMutex) override;
    ndk::ScopedAStatus getDescriptor(Descriptor* _aidl_return) override;
    ndk::ScopedAStatus setParameterSpecific(const Parameter::Specific& specific)
            REQUIRES(mImplMutex) override;
    ndk::ScopedAStatus getParameterSpecific(const Parameter::Id& id, Parameter::Specific* specific)
            REQUIRES(mImplMutex) override;
    IEffect::Status effectProcessImpl(float* in, float* out, int process)
            REQUIRES(mImplMutex) override;
    std::shared_ptr<EffectContext> createContext(const Parameter::Common& common)
            REQUIRES(mImplMutex) override;
    RetCode releaseContext() REQUIRES(mImplMutex) override;

    std::string getEffectName() override { return kEffectName; }

    // downmix override the process because of different input/output sample size requirement
    void process() override;

  private:
    std::shared_ptr<DownmixContext> mContext GUARDED_BY(mImplMutex);
    ndk::ScopedAStatus getParameterDownmix(const Downmix::Tag& tag, Parameter::Specific* specific)
            REQUIRES(mImplMutex);
};
}  // namespace aidl::android::hardware::audio::effect
