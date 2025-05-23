/*
!@
MIT License

Copyright (c) 2020 Skylicht Technology CO., LTD

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This file is part of the "Skylicht Engine".
https://github.com/skylicht-lab/skylicht-engine
!#
*/

#pragma once

#include "IRenderer.h"
#include "ParticleSystem/Particles/CParticle.h"

namespace Skylicht
{
	namespace Particle
	{
		class COMPONENT_API CBillboardAdditiveRenderer : public IRenderer
		{
		public:
			enum EBillboardType
			{
				Billboard,
				RotateY
			};

		protected:
			static const u32 NB_INDICES_PER_QUAD = 6;
			static const u32 NB_VERTICES_PER_QUAD = 4;

			EBillboardType m_billboardType;

		public:
			CBillboardAdditiveRenderer();

			virtual ~CBillboardAdditiveRenderer();

			virtual void getParticleBuffer(IMeshBuffer* buffer);

			virtual void updateParticleBuffer(IMeshBuffer* buffer, CParticle* particles, int num);

			virtual CObjectSerializable* createSerializable();

			virtual void loadSerializable(CObjectSerializable* object);

			inline EBillboardType getBillboardType()
			{
				return m_billboardType;
			}

			inline void setBillboardType(EBillboardType type)
			{
				m_billboardType = type;
			}

			DECLARE_GETTYPENAME(CBillboardAdditiveRenderer)
		};
	}
}