#include "image_frame.h"

#include "assert.h"

ImageFrame::ImageFrame(QWidget* parent, Qt::WindowFlags flags)
  : QFrame()
  , aspect_ratio_(4, 3)
  ,has_image_(false)
{
  connect(this, SIGNAL(delayed_update()), this, SLOT(update()), Qt::QueuedConnection);
}

ImageFrame::~ImageFrame()
{
}

const QImage& ImageFrame::getImage() const
{
  return qimage_;
}

QImage ImageFrame::getImageCopy() const
{
  QImage img;
  qimage_mutex_.lock();
  img = qimage_.copy();
  qimage_mutex_.unlock();
  return img;
}

void ImageFrame::setImage(const QImage& image)//, QMutex* image_mutex)
{
  qimage_mutex_.lock();
  qimage_ = image.copy();
  has_image_ = true ;
  qimage_mutex_.unlock();
  setAspectRatio(qimage_.width(), qimage_.height());
  emit delayed_update();
}

void ImageFrame::resizeToFitAspectRatio()
{
  QRect rect = contentsRect();

  // reduce longer edge to aspect ration
  double width = double(rect.width());
  double height = double(rect.height());
  if (width * aspect_ratio_.height() / height > aspect_ratio_.width())
  {
    // too large width
    width = height * aspect_ratio_.width() / aspect_ratio_.height();
    rect.setWidth(int(width + 0.5));
  }
  else
  {
    // too large height
    height = width * aspect_ratio_.height() / aspect_ratio_.width();
    rect.setHeight(int(height + 0.5));
  }

  // resize taking the border line into account
  int border = lineWidth();
  resize(rect.width() + 2 * border, rect.height() + 2 * border);
}

void ImageFrame::setInnerFrameMinimumSize(const QSize& size)
{
  int border = lineWidth();
  QSize new_size = size;
  new_size += QSize(2 * border, 2 * border);
  setMinimumSize(new_size);
  emit delayed_update();
}

void ImageFrame::setInnerFrameMaximumSize(const QSize& size)
{
  int border = lineWidth();
  QSize new_size = size;
  new_size += QSize(2 * border, 2 * border);
  setMaximumSize(new_size);
  emit delayed_update();
}

void ImageFrame::setInnerFrameFixedSize(const QSize& size)
{
  setInnerFrameMinimumSize(size);
  setInnerFrameMaximumSize(size);
}

void ImageFrame::setAspectRatio(unsigned short width, unsigned short height)
{
  int divisor = greatestCommonDivisor(width, height);
  if (divisor != 0) {
    aspect_ratio_.setWidth(width / divisor);
    aspect_ratio_.setHeight(height / divisor);
  }
}

void ImageFrame::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  qimage_mutex_.lock();
  if (!qimage_.isNull()&&has_image_)
  {
    resizeToFitAspectRatio();
    // TODO: check if full draw is really necessary
    //QPaintEvent* paint_event = dynamic_cast<QPaintEvent*>(event);
    //painter.drawImage(paint_event->rect(), qimage_);
    painter.drawImage(contentsRect(), qimage_);
  } else {
    // default image with gradient
    QLinearGradient gradient(0, 0, frameRect().width(), frameRect().height());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::black);
    painter.setBrush(gradient);
    painter.drawRect(0, 0, frameRect().width() + 1, frameRect().height() + 1);

    painter.setPen(Qt::white);
    QFont current_font ;
    current_font.setPixelSize(24);
    painter.setFont(current_font);
    painter.drawText(this->width()/3,this->height()/2,"No Image");
  }
  qimage_mutex_.unlock();
}

int ImageFrame::greatestCommonDivisor(int a, int b)
{
  if (b==0)
  {
    return a;
  }
  return greatestCommonDivisor(b, a % b);
}

void ImageFrame::setNoImage()
{
    has_image_ = false ;
}
