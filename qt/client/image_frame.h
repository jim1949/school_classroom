#ifndef IMAGEFRAME_H
#define IMAGEFRAME_H

#include <QObject>
#include <QFrame>
#include <QImage>
#include <QLayout>
#include <QLayoutItem>
#include <QMutex>
#include <QPainter>
#include <QRect>
#include <QSize>

class ImageFrame : public QFrame
{ Q_OBJECT
public:
    ImageFrame(QWidget* parent, Qt::WindowFlags flags = 0);

    virtual ~ImageFrame();

    const QImage& getImage() const;

    QImage getImageCopy() const;

    void setNoImage() ;

    void setImage(const QImage& image);

    QRect getAspectRatioCorrectPaintArea();

    void resizeToFitAspectRatio();

    void setInnerFrameMinimumSize(const QSize& size);

    void setInnerFrameMaximumSize(const QSize& size);

    void setInnerFrameFixedSize(const QSize& size);

  signals:

    void delayed_update();

  protected:

    void setAspectRatio(unsigned short width, unsigned short height);

    void paintEvent(QPaintEvent* event);

  private:

    static int greatestCommonDivisor(int a, int b);

    QSize aspect_ratio_;

    QImage qimage_;
    mutable QMutex qimage_mutex_;
    bool has_image_ ;

};

#endif // IMAGEFRAME_H
