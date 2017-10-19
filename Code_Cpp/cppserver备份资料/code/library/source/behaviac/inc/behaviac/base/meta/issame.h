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

#ifndef _BEHAVIAC_BASE_META_ISSAME_H_INCLUDED
#define _BEHAVIAC_BASE_META_ISSAME_H_INCLUDED

namespace behaviac
{
    namespace Meta
    {
        // Compare two type, Answer is true if they are the same
        //
        // IsSame< int32_t, int32_t >::Answer == true
        // IsSame< int32_t, float >::Answer == false
        //

        template< typename First, typename Second >
        struct IsSame
        {
            enum
            {
                Result = 0
            };
        };

        template< typename First >
        struct IsSame< First, First >
        {
            enum
            {
                Result = 1
            };
        };
    }
}

#endif
