#pragma once

#include "ComplexMap.h"
#include "ComplexString.h"

namespace ComplexLibrary
{
	class ComplexXMLNode
	{
	public:
		typedef ComplexMap<ComplexString, ComplexString> ComplexAttributes;

		ComplexXMLNode()
			: next_element(nullptr)
			, parent_element(nullptr)
			, child_element(nullptr)
			, bAnnotation(false)
		{

		}

		virtual ~ComplexXMLNode()
		{
			if (next_element != nullptr)
			{
				delete next_element;
				next_element = nullptr;
			}

			if (child_element != nullptr)
			{
				delete child_element;
				child_element = nullptr;
			}
		}

		bool IsAnnotation() const
		{
			return bAnnotation;
		}

		bool HasNextElement()
		{
			return (next_element != nullptr);
		}

		bool HasChildElement()
		{
			return (child_element != nullptr);
		}

		bool HasParentElement()
		{
			return (parent_element != nullptr);
		}

		ComplexString GetElementName() const
		{
			return element_name;
		}

		ComplexString GetElementValue() const
		{
			return element_value;
		}

		ComplexXMLNode* GetNextElement() const
		{
			return next_element;
		}

		ComplexXMLNode* GetChildElement() const
		{
			return child_element;
		}

		ComplexXMLNode* GetParentElement() const
		{
			return parent_element;
		}

		ComplexAttributes* GetAttributes()
		{
			return &attributes;
		}

		ComplexString GetAttribute(ComplexString key)
		{
			ComplexAttributes::iterator iter;

			iter = attributes.find(key);
			if (iter != attributes.end())
				return iter->value.value;

			return "";
		}

		ComplexString FindKey(int index)
		{
			ComplexAttributes::iterator iter = attributes.begin();

			ComplexString findkey = "";
			int i = 0;
			while (iter != attributes.end())
			{
				if (i == index)
				{
					findkey = iter->value.key;
					break;
				}

				iter++;
				i++;
			}

			return findkey;
		}

		void SetAnnotation(bool annotation)
		{
			bAnnotation = annotation;
		}

		void SetElementName(ComplexString elemname)
		{
			element_name = elemname;
		}

		void SetElementValue(ComplexString elemvalue)
		{
			element_value = elemvalue;
		}

		void SetNextElement(ComplexXMLNode* elem)
		{
			next_element = elem;
		}

		void SetChildElement(ComplexXMLNode* elem)
		{
			child_element = elem;
		}

		void SetParentElement(ComplexXMLNode* elem)
		{
			parent_element = elem;
		}

		void SetAttributes(ComplexAttributes attr)
		{
			attributes = attr;
		}

		void AppendAttribute(ComplexString key, ComplexString value)
		{
			if (attributes.empty())
			{
				attributes.insert(key, value);
				return;
			}

			ComplexAttributes::iterator iter;

			iter = attributes.find(key);
			if (iter != attributes.end())
				iter->value.value = value;
			else
				attributes.insert(key, value);
		}

	private:

		ComplexXMLNode* next_element;
		ComplexXMLNode* parent_element;
		ComplexXMLNode* child_element;

		ComplexAttributes attributes;
		ComplexString element_name;
		ComplexString element_value;
		bool bAnnotation;

	};
}