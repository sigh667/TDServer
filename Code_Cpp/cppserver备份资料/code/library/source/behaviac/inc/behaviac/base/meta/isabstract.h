/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tencent is pleased to support the open source community by making behaviac available.
//
// Copyright (C) 2015 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef BEHAVIAC_ENGINE_SYSTEM_UTILITIES_META_ISABSTRACT_H_INCLUDED
#define BEHAVIAC_ENGINE_SYSTEM_UTILITIES_META_ISABSTRACT_H_INCLUDED

#include "behaviac/base/meta/isclass.h"
#include "behaviac/base/meta/ifthenelse.h"

namespace behaviac
{
    namespace Meta
    {
        template< typename T >
        static No CheckIfAbstract(T(*)[1]);

        template< typename T >
        static Yes CheckIfAbstract(...);

        template< typename T >
        struct IsAbstract
        {
            enum
            {
                Result = IsClass< T >::Result &&
                         sizeof(CheckIfAbstract< T >(0)) == sizeof(Yes)
            };
        };
    }
}

#endif
