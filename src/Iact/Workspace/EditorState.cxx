#include "Iact/Workspace/EditorState.hxx"

#include <QSettings>

#include "Iact/WorkspaceController.hxx"
#include "Iact/InteractiveContext.hxx"

IMPLEMENT_STANDARD_RTTIEXT(EditorState, Standard_Transient)

// ʵ���ļ�
EditorState::EditorState()
{
    workspaceController_ = InteractiveContext::Current()->GetWorkspaceController();
    if(!workspaceController_.IsNull())
    {
    }
}

EditorState::~EditorState()
{
    // �����ź����ӣ��� boost::signals2 �Զ����������ֶ�����
}

void EditorState::SetActiveTool(const std::string& toolId)
{
    activeTool_ = toolId;
    OnActiveToolChanged(toolId); // �����ź�
}

void EditorState::SetRubberbandSelectionMode(RubberbandSelectionMode mode)
{
    rubberbandSelectionMode_ = mode;
    OnRubberbandSelectionModeChanged(mode); // �����ź�
}

void EditorState::SetRubberbandIncludeTouched(bool include)
{
    rubberbandIncludeTouched_ = include;
    OnRubberbandIncludeTouchedChanged(include); // �����ź�
}

void EditorState::SetSnapToGridSelected(bool selected)
{
    if(snapToGridSelected_ != selected)
    {
        snapToGridSelected_ = selected;
        OnSnapToGridSelectedChanged(selected); // �����ź�
    }
}

void EditorState::SetSnapToVertexSelected(bool selected)
{
    if(snapToVertexSelected_ != selected)
    {
        snapToVertexSelected_ = selected;
        OnSnapToVertexSelectedChanged(selected); // �����ź�
        if(!workspaceController_.IsNull()) workspaceController_->InvalidateSelection();
    }
}

void EditorState::SetSnapToEdgeSelected(bool selected)
{
    if(snapToEdgeSelected_ != selected)
    {
        snapToEdgeSelected_ = selected;
        OnSnapToEdgeSelectedChanged(selected); // �����ź�
        if(!workspaceController_.IsNull()) workspaceController_->InvalidateSelection();
    }
}

void EditorState::SetSnappingEnabled(bool enabled)
{
    if(snappingEnabled_ != enabled)
    {
        snappingEnabled_ = enabled;
        OnSnappingEnabledChanged(enabled); // �����ź�
        if(!workspaceController_.IsNull()) workspaceController_->InvalidateSelection();
    }
}

void EditorState::SetDocumentFilterFlags(DocumentFilterFlags flags)
{
    if(documentFilterFlags_ != flags)
    {
        documentFilterFlags_ = flags;
        OnDocumentFilterFlagsChanged(flags); // �����ź�
    }
}

void EditorState::UpdateActiveTool(const std::string& toolId)
{
    activeTool_ = toolId;
    OnActiveToolChanged(toolId); // �����ź�
}

void EditorState::OnWorkspaceControllerPropertyChanged(const std::string& propertyName)
{
}

void EditorState::LoadFromQSettings(const std::shared_ptr<QSettings>& settings)
{
    if(settings)
    {
        // ���û����
        this->SetActiveTool(settings->value("activeTool", "").toString().toStdString());

        // ������Ƥ��ѡ��ģʽ
        QString rubberbandMode = settings->value("rubberbandSelectionMode", "Rectangle").toString();
        if(rubberbandMode == "Polygon")
        {
            this->SetRubberbandSelectionMode(RubberbandSelectionMode::Polygon);
        }
        else
        {
            this->SetRubberbandSelectionMode(RubberbandSelectionMode::Rectangle);
        }

        // ������Ƥ���Ƿ������������
        this->SetRubberbandIncludeTouched(settings->value("rubberbandIncludeTouched", false).toBool());

        // ���ò�׽ѡ��
        this->SetSnapToGridSelected(settings->value("snapToGridSelected", false).toBool());
        this->SetSnapToVertexSelected(settings->value("snapToVertexSelected", false).toBool());
        this->SetSnapToEdgeSelected(settings->value("snapToEdgeSelected", false).toBool());
        this->SetSnappingEnabled(settings->value("snappingEnabled", false).toBool());

        // �����ĵ���������־��������������ʾ��
        int filterFlags = settings->value("documentFilterFlags", 0).toInt();
        this->SetDocumentFilterFlags(static_cast<DocumentFilterFlags>(filterFlags));
    }
}

void EditorState::SaveToQSettings(const std::shared_ptr<QSettings>&settings) const
{
    // �����
    settings->setValue("activeTool", QString::fromStdString(this->GetActiveTool()));

    // ��Ƥ��ѡ��ģʽ
    const QString modeStr = (this->GetRubberbandSelectionMode() == RubberbandSelectionMode::Polygon)
        ? "Polygon" : "Rectangle";
    settings->setValue("rubberbandSelectionMode", modeStr);

    // ������������
    settings->setValue("rubberbandIncludeTouched", this->IsRubberbandIncludeTouched());

    // ��׽����
    settings->setValue("snapToGridSelected", this->IsSnapToGridSelected());
    settings->setValue("snapToVertexSelected", this->IsSnapToVertexSelected());
    settings->setValue("snapToEdgeSelected", this->IsSnapToEdgeSelected());
    settings->setValue("snappingEnabled", this->IsSnappingEnabled());

    // �ĵ���������־
    settings->setValue("documentFilterFlags", static_cast<int>(this->GetDocumentFilterFlags()));

    settings->sync(); // �������浽����
}
