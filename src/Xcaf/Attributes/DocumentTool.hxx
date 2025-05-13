// class DocumentTool

#ifndef _DocumentTool_Header_File_
#define _DocumentTool_Header_File_

#include <TDataStd_GenericEmpty.hxx>
#include <TDF_Label.hxx>
#include <Standard_GUID.hxx>
#include <TDocStd_Document.hxx>
#include <TDF_Attribute.hxx>
#include <TDF_DataSet.hxx>
#include <TDF_RelocationTable.hxx>
#include <TDF_CopyTool.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_AttributeIterator.hxx>

#include "Core/CoreContext.hxx"

class DocumentTool : public TDataStd_GenericEmpty
{
public:
	static const Standard_GUID GetID();

	static Handle(DocumentTool) Set(
		const TDF_Label& L,
		const Standard_Boolean IsAcces = true);

	bool IsCoreDocument(const Handle(TDocStd_Document)& D);

	static TDF_Label DocLabel(const TDF_Label& acces = CoreContext::Current()->MainLabel());

	static TDF_Label EntitysLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

	static TDF_Label ShapesLabel(const TDF_Label& acces = CoreContext::Current()->MainLabel());

	static TDF_Label Shape2dsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

	static TDF_Label SketchsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

	static TDF_Label BoxsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

	static TDF_Label InteractiveEntitysLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

	static TDF_Label BodysLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

	static TDF_Label WorkspacesLabel(const TDF_Label& acces = CoreContext::Current()->MainLabel());

	static TDF_Label PointsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

	static TDF_Label TrashLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

	static TDF_Label SketchSegmentsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchConstraintsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchElementsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchElementSegmentsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchElementPointsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchSegmentLinesLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchSegmentCirclesLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchSegmentArcsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());
    
    static TDF_Label SketchSegmentCircleArcsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchSegmentBeziersLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchSegmentEllipsesLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchSegmentEllipticalArcsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchConstraintVerticalsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchConstraintParallelsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());
    
    static TDF_Label SketchConstraintHorizontalsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());
    
    static TDF_Label SketchConstraintLengthsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());
    
    static TDF_Label SketchConstraintAnglesLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());
    
    static TDF_Label SketchConstraintEqualsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());
    
    static TDF_Label SketchConstraintFixedsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());
    
    static TDF_Label SketchConstraintHorizontalDistancesLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchConstraintPerpendicularsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchConstraintPointOnMidpointsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchConstraintPointOnSegmentsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchConstraintRadiusLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());
    
    static TDF_Label SketchConstraintSmoothCornersLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchConstraintTangentsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());
    
    static TDF_Label SketchConstraintVerticalDistancesLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    static TDF_Label SketchConstraintConcentricsLabel(const TDF_Label& access = CoreContext::Current()->MainLabel());

    // �ƶ���ǩ�����ӱ�ǩ�����¸���ǩ��
    static bool MoveLabel(const TDF_Label& sourceLabel, const TDF_Label& newParentLabel)
    {
        if(!CopyLabel(sourceLabel, newParentLabel))
        {
            return false;
        }

        DeleteLabel(sourceLabel);

        return true;
    }

    // ���Ʊ�ǩ�����ӱ�ǩ�����¸���ǩ��
    static bool CopyLabel(const TDF_Label& sourceLabel, const TDF_Label& newParentLabel)
    {
        // ����ǩ��Ч��
        if(sourceLabel.IsNull() || newParentLabel.IsNull())
        {
            return false;
        }

        // �������ӱ�ǩ��Ϊ����Ŀ��
        TDF_Label newLabel = newParentLabel;

        // ��ʼ�� TDF_DataSet
        Handle(TDF_DataSet) dataSet = new TDF_DataSet();
        AddLabelWithChildren(sourceLabel, dataSet);
        dataSet->Roots().Append(sourceLabel); // ���ø���ǩ

        // ��ʼ�� TDF_RelocationTable
        Handle(TDF_RelocationTable) relocationTable = new TDF_RelocationTable();
        relocationTable->LabelTable().Bind(sourceLabel, newLabel); // Ԥ�󶨸���ǩ

        // ִ�и���
        TDF_CopyTool::Copy(dataSet, relocationTable);
        return true;
    }

	static void DeleteLabel(const TDF_Label& label)
	{
		if(label.IsNull())
		{
			return;
		}

		// ɾ����ǩ�����ӱ�ǩ��
		label.ForgetAllAttributes(true); // true ��ʾ�ݹ�ɾ���ӱ�ǩ
	}

    static std::string GetName(const TDF_Label& label)
    {
		if(label.IsNull())
		{
			return "";
		}

		// ��ȡ��ǩ������
		Handle(TDataStd_Name) nameAttr;
		if(label.FindAttribute(TDataStd_Name::GetID(), nameAttr))
		{
			return OcctHelper::ExtendedStringToStdString(nameAttr->Get());
		}

		return "";
    }

private:
    // �ݹ���ӱ�ǩ�����ӱ�ǩ�� TDF_DataSet
    static void AddLabelWithChildren(const TDF_Label& label, Handle(TDF_DataSet)& dataSet)
    {
        if(label.IsNull())
        {
            return;
        }

        // ��ӵ�ǰ��ǩ
        dataSet->AddLabel(label);

		// ����ӱ�ǩ�µ�����Attribute

        // ��ӵ�ǰ��ǩ����������
        for(TDF_AttributeIterator attrIt(label); attrIt.More(); attrIt.Next())
        {
            dataSet->AddAttribute(attrIt.Value());
        }

        // �ݹ�����ӱ�ǩ
        for(TDF_ChildIterator it(label); it.More(); it.Next())
        {
            AddLabelWithChildren(it.Value(), dataSet);
        }
    }

public:
	DocumentTool();

	void Init() const;

	const Standard_GUID& ID() const override;

	bool AfterRetrieval(const bool forceIt = false)
		override;

	DEFINE_DERIVED_ATTRIBUTE(DocumentTool, TDataStd_GenericEmpty)
};

DEFINE_STANDARD_HANDLE(DocumentTool, TDF_Attribute)

#endif  // _DocumentTool_Header_File_
