//! @file HudManager.hxx
//! @brief ���� HUD Ԫ�ص������벼��

#ifndef IACT_HUDMANAGER_HXX_
#define IACT_HUDMANAGER_HXX_

#include <algorithm>
#include <vector>

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

#include <Standard_DefineHandle.hxx>
#include <Standard_Transient.hxx>

#include "Iact/HudElements/HudElement.hxx"

class HudManager : public Standard_Transient
{
public:
    HudManager() = default;

    //! ��ʼ�� HUD �������ⲿӦ�ֶ����� layout ����ʽ��
    void SetHudContainer(QWidget* container)
    {
        if(!container)
            return;

        m_hudContainer = container;
    }

	void SetHintMessager(QLabel* theMessager)
	{
		if(!theMessager)
			return;

        m_hintMessageLabel = theMessager;
	}

    //! ��� HUD Ԫ�أ������ظ���
    void AddElement(HudElement* theElement)
    {
        if(!m_hudContainer || !theElement)
            return;

        if(std::find(m_hudElements.begin(), m_hudElements.end(), theElement) != m_hudElements.end())
            return;

        if(auto layout = layout_())
        {
            layout->addWidget(theElement);
            theElement->show();

            m_hudElements.push_back(theElement);
            m_hudContainer->show();
        }
    }

    //! �Ƴ� HUD Ԫ��
    void RemoveElement(HudElement* theElement)
    {
        if(!m_hudContainer || !theElement)
            return;

        auto it = std::find(m_hudElements.begin(), m_hudElements.end(), theElement);
        if(it == m_hudElements.end())
            return;

        if(auto layout = layout_())
        {
            layout->removeWidget(theElement);
            theElement->hide();
            theElement->setParent(nullptr);

            m_hudElements.erase(it);

            if(m_hudElements.empty())
                m_hudContainer->hide();
        }
    }

    //! ������� HUD Ԫ��
    void ClearElements()
    {
        if(!m_hudContainer)
            return;

        if(auto layout = layout_())
        {
            for(HudElement* element : m_hudElements)
            {
                layout->removeWidget(element);
                element->hide();
                element->setParent(nullptr);
            }
            m_hudElements.clear();
            m_hudContainer->hide();
        }
    }

    void SetHintMessage(const std::any& owner, const std::string& message)
    {
        if(!myHintMessage.empty() && owner.has_value())
        {
            // ��ȡָ���ַ���Ƚ�
            try
            {
                void* ownerPtr = std::any_cast<void*>(owner);
                void* currentOwnerPtr = myHintMessageOwner.has_value() ? std::any_cast<void*>(myHintMessageOwner) : nullptr;
                if(ownerPtr != currentOwnerPtr)
                {
                    return; // ָ���ַ��ƥ�䣬������
                }
            }
            catch(const std::bad_any_cast&)
            {
                return; // ���Ͳ�ƥ�䣬������
            }
        }
        myHintMessage = message;
        myHintMessageOwner = message.empty() ? std::any() : owner;

		if(m_hintMessageLabel)
		{
			m_hintMessageLabel->setText(QString::fromStdString(myHintMessage));
			m_hintMessageLabel->show();
		}
    }

    //! ���ص�ǰ����ӵ�Ԫ��
    const std::vector<HudElement*>& Elements() const { return m_hudElements; }

private:
    QVBoxLayout* layout_() const
    {
        return m_hudContainer ? qobject_cast<QVBoxLayout*>(m_hudContainer->layout()) : nullptr;
    }

private:
    std::vector<HudElement*> m_hudElements;          //!< ����ӵ� HUD Ԫ���б�
	QLabel* m_hintMessageLabel = nullptr;           //!< ��ʾ��Ϣ��ǩ
    QWidget* m_hudContainer = nullptr;                //!< �ⲿ����� HUD ����
    std::any myHintMessageOwner;
	std::string myHintMessage;
};

// ����������
DEFINE_STANDARD_HANDLE(HudManager, Standard_Transient)

#endif // IACT_HUDMANAGER_HXX_
