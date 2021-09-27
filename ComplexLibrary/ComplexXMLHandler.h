#pragma once

#include "ComplexXMLParsor.h"
#include "ComplexVector.h"

namespace ComplexLibrary
{
	class ComplexXMLHandler;
	class ComplexXMLTreeElem;

	typedef ComplexMap<ComplexString, ComplexString> AttributeContainer;
	typedef ComplexVector<ComplexXMLTreeElem*> ElementContainer;

	//	트리형식의 xml데이터 포맷
	class ComplexXMLTreeElem
	{
		friend class ComplexXMLHandler; // 임시

	public:

		ComplexXMLTreeElem()
			: m_ElemLevel(0)
			, m_ElemIdx(0)
		{

		}

		virtual ~ComplexXMLTreeElem()
		{
			for (int i = 0; i < (int)m_childElemVector.size(); i++)
			{
				ComplexXMLTreeElem* childElem = m_childElemVector.at(i);
				delete childElem;
				childElem = nullptr;
			}
			m_childElemVector.clear();
		}

		ComplexXMLTreeElem*			m_parentElem;		// 해당 엘리먼트의 부모 엘리먼트값
		AttributeContainer			m_attrMap;			// xml 엘리먼트의 속성값들을 담는 맵
		ElementContainer			m_childElemVector;	// xml 엘리먼트의 하위 엘리먼트들을 담는 벡터
		ComplexString				m_tagName;			// 해당 엘리먼트의 태그이름
		int							m_ElemLevel;		// 엘리먼트의 뎁스 
		int							m_ElemIdx;			// 부모 엘리먼트로 부터 자신이 몇번째 하위 엘리먼트인지 나타내는 엘리먼트 인덱스

	};


	class ComplexXMLHandler
	{

	public:
		ComplexXMLHandler()
			: m_rootElement(new ComplexXMLTreeElem)
		{

		}

		virtual ~ComplexXMLHandler()
		{
			Release();
		}

		/*
		* xml을 로드하여 m_rootElement에 모든 값을 담음
		*/
		bool Load(ComplexString strFilePath)
		{
			ComplexXMLParsor mark;

			if (mark.Load(strFilePath) == false)
			{
				Release();
				return false;
			}

			if (m_rootElement == nullptr)
				m_rootElement = new ComplexXMLTreeElem;

			m_strFilePath = strFilePath;
			InitTreeData(mark);

			return true;
		}

		/*
		* m_curElement가 가르키고있는 위치 엘리먼트를 리턴
		*/
		ComplexXMLTreeElem* GetCurElemInstance()
		{
			if (m_curElement == nullptr)
				m_curElement = m_rootElement;

			return m_curElement;
		}

		/*
		* 태그이름과 엘리먼트 인덱스로 엘리먼트를 찾음
		* 발견하면 m_curTagElement에 대입
		*/
		bool FindElemPos(ComplexString strTagName, int tagIdx)
		{
			if (m_rootElement)
			{
				if (m_rootElement->m_tagName == strTagName && m_rootElement->m_ElemIdx == tagIdx)
				{
					m_curElement = m_rootElement;
					return true;
				}
				else
				{
					return FindElem(m_rootElement->m_childElemVector, strTagName, tagIdx);
				}
			}
			return false;
		}

		/*
		* 특정 엘리먼트를 m_rootElement에서 모두 지움
		*/
		bool DeleteCertainElement(ComplexString strTagName)
		{
			if (m_rootElement == nullptr)
				return false;

			return DeleteElement(&m_rootElement->m_childElemVector, strTagName);
		}

		/*
		* 입력한 m_rootElement를 xml 경로에 저장함.
		* 경로가 설정 안되어있으면 load했을 때 저장한 경로로 설정됨
		*/
		bool Save(ComplexString strFilePath = "")
		{
			if (m_rootElement == nullptr)
				return false;

			bool bAnotherFile = false;

			if (!strFilePath.IsEmpty())
				bAnotherFile = true;

			ComplexXMLParsor markup;

			markup.AddElement(m_rootElement->m_tagName);
			AttributeContainer attrMap = m_rootElement->m_attrMap;
			AttributeContainer::iterator iter = attrMap.begin();
			while (iter != attrMap.end())
			{
				markup.AddAttribute(iter->value.key, iter->value.value);
				iter++;
			}
			markup.IntoElement();
			ExecuteTreeTag(markup, m_rootElement->m_childElemVector);
			markup.OutOfElement();

			markup.Save(bAnotherFile ? strFilePath : m_strFilePath);

			return true;
		}

		/*
		* m_rootElement값을 설정함
		* xml을 저장할 때 사용
		*/
		void SetRootElement(ComplexXMLTreeElem* overWriteRootElem)
		{
			m_rootElement = overWriteRootElem;
		}

		/*
		* xml을 로드했을 때 데이터 참조를위해 사용
		*/
		ComplexXMLTreeElem* GetRootElement() const
		{
			return m_rootElement;
		}

		/*
		* 모든 데이터 사용이 완료되었을 때 해당 트리엘리먼트 메모리를 삭제함
		*/
		void Release()
		{
			if (m_rootElement)
			{
				delete m_rootElement;
				m_rootElement = nullptr;
			}

			m_curElement = nullptr;
			m_strFilePath = "";
		}

	private:

		ComplexXMLTreeElem*		 m_rootElement;	// 데이터 입력이나 로드할 때 사용하는 실질적 데이터참조 객체

		ComplexXMLTreeElem*		 m_curElement;		// 로드하였을 때 찾고싶은 데이터를 발견했을 때 사용하는 객체

		ComplexString			 m_strFilePath;		// 로드할 때 저장되는 xml 파일경로

		void InitTreeData(ComplexXMLParsor& mark)
		{
			if (mark.FindElement())
			{
				m_rootElement->m_parentElem = nullptr;
				m_rootElement->m_tagName = mark.GetElementName();
				m_rootElement->m_ElemIdx = 0;
				m_rootElement->m_ElemLevel = 0;
				m_curElement = m_rootElement;
				mark.IntoElement();

				LoadElem(mark, m_rootElement, 1);
				mark.OutOfElement();
			}

		}

		void LoadElem(ComplexXMLParsor& mark, ComplexXMLTreeElem* parentTag, int tagLevel)
		{
			int i = 0;
			while (mark.FindElement())
			{
				ComplexXMLTreeElem* curTag = new ComplexXMLTreeElem;
				AttributeContainer attrMap;

				int j = 0;
				while (mark.GetAttributeName(j).IsEmpty() == false)
				{
					attrMap.insert(mark.GetAttributeName(j), mark.GetAttribute(mark.GetAttributeName(j)));
					j++;
				}
				curTag->m_parentElem = parentTag;
				curTag->m_tagName = mark.GetElementName();
				curTag->m_ElemIdx = i;
				curTag->m_ElemLevel = tagLevel;
				curTag->m_attrMap = attrMap;

				m_curElement = curTag;

				if (mark.HasChildElement())
				{
					mark.IntoElement();
					LoadElem(mark, curTag, tagLevel + 1);
					mark.OutOfElement();
				}
				parentTag->m_childElemVector.push_back(curTag);

				i++;
			}

		}

		bool FindElem(ElementContainer findElemContainer, ComplexString strTagName, int tagIdx)
		{
			for (int i = 0; i < (int)findElemContainer.size(); i++)
			{
				ComplexXMLTreeElem* childElem = findElemContainer.at(i);
				if (childElem->m_tagName == strTagName && childElem->m_ElemIdx == tagIdx)
				{
					m_curElement = childElem;
					return true;
				}
				else
				{
					if (FindElem(childElem->m_childElemVector, strTagName, tagIdx))
					{
						return true;
					}
				}
			}
			return false;
		}

		void ExecuteTreeTag(ComplexXMLParsor& mark, ElementContainer executeTagContainer)
		{
			for (int i = 0; i < (int)executeTagContainer.size(); i++)
			{
				ComplexXMLTreeElem* executeTag = executeTagContainer.at(i);
				AttributeContainer attrMap = executeTag->m_attrMap;

				mark.AddElement(executeTag->m_tagName);
				if (!attrMap.empty())
				{
					AttributeContainer::iterator iter = attrMap.begin();
					while (iter != attrMap.end())
					{
						mark.AddAttribute(iter->value.key, iter->value.value);
						iter++;
					}
				}

				mark.IntoElement();
				ExecuteTreeTag(mark, executeTag->m_childElemVector);
				mark.OutOfElement();
			}
		}

		bool DeleteElement(ElementContainer* tagetContainer, ComplexString strTagName)
		{
			bool bReturn = false;
			int i = 0;

			for (int i = 0; i < (int)tagetContainer->size();)
			{
				ComplexXMLTreeElem* childElem = tagetContainer->at(i);
				bReturn = DeleteElement(&childElem->m_childElemVector, strTagName);
				if (childElem->m_tagName == strTagName)
				{
					tagetContainer->erase(i);
					delete childElem;
					childElem = nullptr;
					bReturn = true;
				}
				else
				{
					i++;
				}
			}
			return bReturn;
		}

	};
}