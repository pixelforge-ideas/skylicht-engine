/*
!@
MIT License

Copyright (c) 2022 Skylicht Technology CO., LTD

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

#include "CObjectSerializable.h"
#include <functional>

namespace Skylicht
{
	class SKYLICHT_API CArraySerializable : public CObjectSerializable
	{
	public:
		CArraySerializable(const char* name);

		CArraySerializable(const char* name, CObjectSerializable* parent);

		virtual ~CArraySerializable();

		template<class T>
		T getElementValue(int i, T defaultValue)
		{
			T* v = dynamic_cast<T*>(getElement(i));
			if (v == NULL)
				return defaultValue;
			return *v;
		}

		int getElementCount()
		{
			return getNumProperty();
		}

		CValueProperty* getElement(int i)
		{
			return getPropertyID(i);
		}

		void removeByIndex(int index);

		void clear();

		virtual bool haveCreateElementFunction()
		{
			return false;
		}

		virtual CValueProperty* createElement()
		{
			return NULL;
		}

		bool resize(int count);

	protected:

		void autoName();
	};

	template <class T>
	class SKYLICHT_API CArrayTypeSerializable : public CArraySerializable
	{
	public:
		std::function<void(CValueProperty*)> OnCreateElement;

	public:
		CArrayTypeSerializable(const char* name) :
			CArraySerializable(name)
		{

		}

		CArrayTypeSerializable(const char* name, CObjectSerializable* parent) :
			CArraySerializable(name, parent)
		{

		}

		virtual bool haveCreateElementFunction()
		{
			return true;
		}

		virtual CValueProperty* createElement()
		{
			T* t = new T();
			CValueProperty* element = dynamic_cast<CValueProperty*>(t);
			if (element == NULL)
			{
				delete t;
				return NULL;
			}

			char name[32];
			sprintf(name, "[%d]", (int)m_value.size());

			addProperty(element);
			autoRelease(element);

			element->Name = name;

			if (OnCreateElement != nullptr)
			{
				OnCreateElement(element);
			}

			return element;
		}
	};
}