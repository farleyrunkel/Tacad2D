// Copyright [2024] SunCAD

#ifndef IACT_VIEWPORT_VIEWPORTPANEL_H_
#define IACT_VIEWPORT_VIEWPORTPANEL_H_

// Qt includes
#include <QFrame>
#include <QVBoxLayout>
#include <QWidget>

// Project includes
#include "Iact/HudElements/HudManager.hxx"
#include "Iact/InteractiveContext.hxx"
#include "Iact/Viewport/ViewportWidget.hxx"
#include "Iact/ViewportController.hxx"
#include "Iact/WorkspaceController.hxx"

/// @brief ViewportPanel class
class ViewportPanel : public QWidget
{
	Q_OBJECT

public:
	explicit ViewportPanel(QWidget* parent = nullptr);

	Handle(ViewportController) viewportController() const
	{
		return myViewportController;
	}

private:
	void initHudContainer()
	{
		myHudContainer = new QFrame(this);
		myHudContainer->setFrameShape(QFrame::NoFrame);
		myHudContainer->setMouseTracking(true);
		myHudContainer->setVisible(false);
		myHudContainer->setLayout(new QVBoxLayout);
		myHudContainer->setAutoFillBackground(false);
		myHudContainer->setStyleSheet("background-color: rgba(128, 128, 128, 0.5);");
	}

	void setupConnection()
	{
		InteractiveContext::Current()->viewportControllerChanged.connect(
			std::bind(&ViewportPanel::onViewportControllerChanged, this, std::placeholders::_1));
		InteractiveContext::Current()->workspaceControllerChanged.connect(
			std::bind(&ViewportPanel::onWorkspaceControllerChanged, this, std::placeholders::_1));
	}

	void onWorkspaceControllerChanged(const Handle(WorkspaceController)& WCer)
	{
		if(myHudContainer != nullptr)
		{
			auto aHudManager = WCer->GetHudManager();
			aHudManager->SetHudContainer(myHudContainer);
		}
	}

	void onViewportControllerChanged(const Handle(ViewportController)& theCV)
	{
		if(myViewportController == theCV)
		{
			return;
		}
		myViewportController = theCV;

		if(myViewportHwndHost != nullptr)
		{
			layout()->removeWidget(myViewportHwndHost);
			myViewportHwndHost->deleteLater();
		}

		myViewportHwndHost = new ViewportWidget(theCV);

		layout()->addWidget(myViewportHwndHost);
		myViewportHwndHost->setFocus();
	}
protected:
	virtual void mouseMoveEvent(QMouseEvent* event) override;

void updateHud(const QPointF& pos) {
        if (!myHudContainer) return; // ��ֹ��ָ��

        // ��ȡ HUD �����ĳߴ�
        int hudWidth = myHudContainer->width();
        int hudHeight = myHudContainer->height();

        // ��ȡ�����ڵĳߴ磨������һ�������ڣ�
        QWidget* parent = myHudContainer->parentWidget();
        int parentWidth = parent ? parent->width() : 1920; // Ĭ��ֵ�������ʵ�ʵ���
        int parentHeight = parent ? parent->height() : 1080;

        // ����λ�ã��ο� C# �߼�
        int x = static_cast<int>(pos.x());
        int y = static_cast<int>(pos.y());

        // ���� x������Ҳ����㹻�ռ䣬���� pos.x�������Ƶ����
        if ((parentWidth - hudWidth - x) > 0) {
            x += 10; // �Ҳ�ƫ�ƣ��ο����ԭʼ���룩
        } else {
            x -= hudWidth + 10; // �Ƶ���ಢ�������
        }

        // ���� y������Ϸ����㹻�ռ䣬���� pos.y �Ϸ��������·�
        if ((y - hudHeight) > 0) {
            y -= hudHeight + 10; // �Ϸ�ƫ�ƣ��ο����ԭʼ���룩
        } else {
            y += 10; // �Ƶ��·����������
        }

        // ȷ���������߽�
        x = std::max(0, std::min(x, parentWidth - hudWidth));
        y = std::max(0, std::min(y, parentHeight - hudHeight));

        // ���� HUD λ��
        myHudContainer->move(x, y);
		myHudContainer->raise();
    }

public:
	Handle(ViewportController) myViewportController = nullptr;
	ViewportWidget* myViewportHwndHost = nullptr;
	QFrame* myHudContainer = nullptr;
};


#endif  // IACT_VIEWPORT_VIEWPORTPANEL_H_
