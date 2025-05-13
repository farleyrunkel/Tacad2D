// Copyright [2025] TaCAD

// Own include
#include "Occt/OcctHelper/PixMapHelper.hxx"

// Qt include
#include <QImage>

// Occt includes
#include <Image_PixMap.hxx>
#include <Image_Format.hxx>

Handle(Image_PixMap) PixMapHelper::convertFromBitmap(const QImage& image)
{
    if (image.isNull()) {
        throw std::runtime_error("Failed to convert QPixmap to QImage.");
    }

    // ȷ�� Image_PixMap ��ʽ
    Image_Format imageFormat;
    switch (image.format()) {
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        imageFormat = Image_Format_RGBA;
        break;
    case QImage::Format_RGB32:
        imageFormat = Image_Format_RGB;
        break;
    case QImage::Format_RGB888:
        imageFormat = Image_Format_RGB;
        break;
    case QImage::Format_Indexed8:
        imageFormat = Image_Format_Gray;
        break;
    default:
        throw std::runtime_error("Unsupported QImage format.");
    }

    // ���� Image_PixMap ����
    Handle(Image_PixMap) pixmap = new Image_PixMap;
    if (!pixmap->InitTrash(imageFormat, image.width(), image.height())) {
        throw std::runtime_error("Failed to Initialize Image_PixMap.");
    }

    // ��������
    for (int y = 0; y < image.height(); ++y) {
        const uchar* srcLine = image.constScanLine(y);
        uchar* destLine = (uchar*)pixmap->ChangeRow(y);
        std::memcpy(destLine, srcLine, image.bytesPerLine());
    }

    return pixmap;
}
