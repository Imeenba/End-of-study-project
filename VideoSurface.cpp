#include <QAbstractVideoSurface>
#include <QVideoFrame>
#include <QImage>

class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    VideoSurface(QObject *parent = nullptr) : QAbstractVideoSurface(parent) {}

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
        QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override
    {
        Q_UNUSED(handleType);
        return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB32;
    }

    bool present(const QVideoFrame &frame) override
    {
        if (frame.isValid()) {
            QVideoFrame cloneFrame(frame);
            cloneFrame.map(QAbstractVideoBuffer::ReadOnly);
            QImage img(cloneFrame.bits(),
                       cloneFrame.width(),
                       cloneFrame.height(),
                       QVideoFrame::imageFormatFromPixelFormat(cloneFrame.pixelFormat()));
            cloneFrame.unmap();

            emit framePresented(img); // Émettre le signal pour transmettre l'image à QVideoWidget
        }
        return true;
    }

    bool start(const QVideoSurfaceFormat &format) override
    {
        return QAbstractVideoSurface::start(format);
    }

signals:
    void framePresented(const QImage &image);
};
