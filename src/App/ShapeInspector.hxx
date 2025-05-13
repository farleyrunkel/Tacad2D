// Copyright [2024] FaCAD

#ifndef APP_ShapeInspector_H_
#define APP_ShapeInspector_H_

// Qt includes
#include <QWidget>

class ShapeInspector : public QWidget
{
    Q_OBJECT

public:
    explicit ShapeInspector(QWidget* parent = nullptr)
        : QWidget(parent) {}
    ~ShapeInspector() override = default;

private:
    void setupUi() {}
};

#endif  // APP_ShapeInspector_H_
