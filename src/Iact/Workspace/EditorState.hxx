//! class EditorState
//! 

#ifndef IACT_EDITORSTATE_HXX
#define IACT_EDITORSTATE_HXX

#include <string>
#include <vector>

#include <QSettings>

#include <AIS_InteractiveObject.hxx>
#include <gp_Pnt.hxx>
#include <Standard_DefineHandle.hxx>
#include <Standard_Transient.hxx>
#include <TopoDS_Shape.hxx>

#include "Iact/WorkspaceController.hxx"

// ǰ������
class SnapInfo;
class ViewportController;

// ������
DEFINE_STANDARD_HANDLE(EditorState, Standard_Transient)

// �����ö�����ͣ���������Ҫ���֣�
enum class RubberbandSelectionMode { Rectangle, Polygon  };
enum class DocumentFilterFlags { None = 0, /* ������־ */ };

// EditorState ��
class EditorState : public Standard_Transient
{
public:
    // ���캯��
    EditorState();

    // ��������
    virtual ~EditorState();

    // RTTI ֧��
    DEFINE_STANDARD_RTTIEXT(EditorState, Standard_Transient)

    // ��ȡ�����õ�ǰ�����ID
    std::string GetActiveTool() const { return activeTool_; }
    void SetActiveTool(const std::string& toolId);

    // ��Ƥ��ѡ��ģʽ
    RubberbandSelectionMode GetRubberbandSelectionMode() const { return rubberbandSelectionMode_; }
    void SetRubberbandSelectionMode(RubberbandSelectionMode mode);

    // ��Ƥ��ѡ���Ƿ���������Ķ���
    bool IsRubberbandIncludeTouched() const { return rubberbandIncludeTouched_; }
    void SetRubberbandIncludeTouched(bool include);

    // �Ƿ���������׽
    bool IsSnapToGridSelected() const { return snapToGridSelected_; }
    void SetSnapToGridSelected(bool selected);

    // �Ƿ����ö��㲶׽
    bool IsSnapToVertexSelected() const { return snapToVertexSelected_; }
    void SetSnapToVertexSelected(bool selected);

    // �Ƿ����ñ߲�׽
    bool IsSnapToEdgeSelected() const { return snapToEdgeSelected_; }
    void SetSnapToEdgeSelected(bool selected);

    // �Ƿ����ò�׽����
    bool IsSnappingEnabled() const { return snappingEnabled_; }
    void SetSnappingEnabled(bool enabled);

    // ��ȡ��׽��Ϣ
    SnapInfo* GetSnapInfo() const { return snapInfo_; }

    // �ĵ���������־
    DocumentFilterFlags GetDocumentFilterFlags() const { return documentFilterFlags_; }
    void SetDocumentFilterFlags(DocumentFilterFlags flags);

	void LoadFromQSettings(const std::shared_ptr<QSettings>& settings);
	void SaveToQSettings(const std::shared_ptr<QSettings>& settings) const;

public:
    Signal<void(const std::string&)> OnActiveToolChanged;
    Signal<void(RubberbandSelectionMode)> OnRubberbandSelectionModeChanged;
    Signal<void(bool)> OnRubberbandIncludeTouchedChanged;
    Signal<void(bool)> OnSnapToGridSelectedChanged;
    Signal<void(bool)> OnSnapToVertexSelectedChanged;
    Signal<void(bool)> OnSnapToEdgeSelectedChanged;
    Signal<void(bool)> OnSnappingEnabledChanged;
    Signal<void(SnapInfo*)> OnSnapInfoChanged;
    Signal<void(DocumentFilterFlags)> OnDocumentFilterFlagsChanged;

private:
    // ���»����
    void UpdateActiveTool(const std::string& toolId);

    // ���������������Ա仯�ص�
    void OnWorkspaceControllerPropertyChanged(const std::string& propertyName);

    // ���ݳ�Ա
    std::string activeTool_;                        // ��ǰ�����ID
    RubberbandSelectionMode rubberbandSelectionMode_; // ��Ƥ��ѡ��ģʽ
    bool rubberbandIncludeTouched_ = false;         // ��Ƥ��ѡ���Ƿ��������
    Handle(WorkspaceController) workspaceController_ = nullptr; // ������������
    bool snapToGridSelected_ = false;               // ����׽����
    bool snapToVertexSelected_ = false;             // ���㲶׽����
    bool snapToEdgeSelected_ = false;               // �߲�׽����
    bool snappingEnabled_ = false;                  // ��׽�ܿ���
    SnapInfo* snapInfo_ = nullptr;                  // ��׽��Ϣ
    DocumentFilterFlags documentFilterFlags_ = DocumentFilterFlags::None; // �ĵ���������־
};

#endif  // IACT_EDITORSTATE_HXX