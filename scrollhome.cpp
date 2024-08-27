#include "scrollhome.h"  // 包含自定义头文件 "scrollhome.h"
#include <QDebug>        // 包含 Qt 的调试功能头文件，用于输出调试信息
#include <QPushButton>   // 包含 QPushButton 类，用于创建按钮
#include <QPainter>      // 包含 QPainter 类，用于绘图操作
#include <QLabel>        // 包含 QLabel 类，用于显示标签
#include <QHeaderView>   // 包含 QHeaderView 类，用于控制表头
#include <QPainterPath>  // 包含 QPainterPath 类，用于绘制复杂路径

// ScrollHome 类的构造函数，继承自 QTableWidget
ScrollHome::ScrollHome(QWidget *parent)
    : QTableWidget(parent)
{
    this->parent = parent;  // 初始化父窗口

    // 设置表格的样式：背景透明且无边框
    this->setStyleSheet("background-color: transparent;   border: none; ");

    // 设置字体和调色板
    font.setFamily("microsoft yahei");  // 设置字体为微软雅黑
    font.setWeight(50);                 // 设置字体权重
    palette.setColor(QPalette::Text, QColor(255, 255, 255));  // 设置字体颜色为白色

    // 设置表格的属性
    this->setFocusPolicy(Qt::NoFocus);  // 禁用焦点
    this->setShowGrid(false);           // 不显示网格线
    this->setSelectionMode(QAbstractItemView::NoSelection); // 禁用选择
    this->setSelectionBehavior(QAbstractItemView::SelectItems); // 选择行为设置为选择单元格
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);   // 禁用编辑

    // 设置滚动条的属性
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 隐藏垂直滚动条
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏水平滚动条
    this->verticalHeader()->hide();  // 隐藏垂直表头
    this->horizontalHeader()->hide(); // 隐藏水平表头

    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel); // 设置滚动模式为像素滚动
}

// 创建一个新的应用按钮
QPushButton* ScrollHome::newAppButton(QWidget* app, AppRes& appRes)
{
    QPushButton *btn = new QPushButton(app);  // 创建一个新的 QPushButton

    // 创建并设置图标
    QPixmap icon(iconSize, iconSize);  // 创建一个指定大小的 QPixmap 作为按钮的图标
    icon.fill(Qt::transparent);        // 填充图标为透明色
    qDebug() << appRes.url;            // 输出应用资源的 URL 供调试
    QPixmap image(appRes.url);         // 加载指定 URL 的图像

    // 调整图像大小，保持纵横比并使用平滑转换
    image = image.scaled(iconSize, iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QPainter painter(&icon);  // 创建 QPainter 对象，用于在图标上绘制内容
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);  // 设置抗锯齿和平滑转换

    QPainterPath path;  // 创建一个路径，用于绘制圆角矩形
    path.addRoundedRect(0, 0, iconSize, iconSize, iconSize/4.5, iconSize/4.5);  // 添加圆角矩形到路径
    painter.setClipPath(path);  // 设置裁剪路径

    painter.drawPixmap(0, 0, iconSize, iconSize, image);  // 绘制缩放后的图像

    btn->setIcon(icon);  // 将绘制的图标设置为按钮的图标
    btn->setIconSize(QSize(iconSize, iconSize));  // 设置按钮图标的大小

    btn->setFixedSize(iconSize, iconSize);  // 设置按钮的固定大小

    // 设置按钮的样式
    btn->setStyleSheet("QPushButton{ background: transparent; border: none; } \
                        QPushButton:hover   { padding-bottom: 2px; } \
                        QPushButton:pressed { padding-top: 3px; }");

    // 强制刷新按钮的样式
    btn->style()->unpolish(btn);
    btn->style()->polish(btn);

    btn->setProperty("resIndex", appRes.resIndex);  // 将应用资源索引设置为按钮的属性
    connect(btn, SIGNAL(clicked()), this, SLOT(onIcon()));  // 连接按钮的点击信号到槽函数 onIcon()

    btn->setFocusPolicy(Qt::NoFocus);  // 禁用按钮的焦点

    return btn;  // 返回创建的按钮
}

// 创建一个新的应用图标
QWidget* ScrollHome::newAppIcon(AppRes& appRes)
{
    QWidget *app = new QWidget(this);  // 创建一个新的 QWidget 作为图标容器

    app->setFixedSize(itemSize);  // 设置图标容器的固定大小

    QPushButton *btn = newAppButton(app, appRes);  // 创建一个新的应用按钮
    btn->move(iconSpacing/2, iconSpacing/2);  // 设置按钮的位置

    QLabel *label = new QLabel(appRes.appName, app);  // 创建一个新的 QLabel 显示应用名称
    label->setFont(font);  // 设置标签的字体
    label->setAlignment(Qt::AlignTop | Qt::AlignHCenter);  // 设置标签的对齐方式为顶部居中
    label->setGeometry(0, iconSize + btn->y() + 4, itemSize.width(), itemSize.height() - iconSize - btn->y());  // 设置标签的位置和大小
    label->setPalette(palette);  // 设置标签的调色板

    return app;  // 返回创建的图标容器
}

// 初始化 ScrollHome 界面
void ScrollHome::InitHome(int iconRow, int iconSpacing, int widgetWidth, int widgetHeight)
{
    this->iconRow = iconRow;  // 初始化图标列数
    this->iconSpacing = iconSpacing;  // 初始化图标间距
    this->setFixedSize(widgetWidth, widgetHeight);  // 设置 ScrollHome 的固定大小
    this->viewport()->setFixedHeight(widgetHeight);  // 设置视口的高度

    // 计算图标的大小
    iconSize = (widgetHeight - iconSpacing * iconRow) / iconRow;
    font.setPixelSize(iconSize / 5.0);  // 设置字体大小
    itemSize = QSize(iconSize + iconSpacing, iconSize + iconSpacing + font.pixelSize() + 10);  // 设置图标项的大小

    this->setRowCount(iconRow);  // 设置列数
    for(int i = 0; i < iconRow; ++i)
    {
        this->setRowHeight(i, itemSize.height());  // 设置每列的宽度
    }

    this->clearContents();  // 清除表格内容
    int columnCount = g_appResSize / iconRow + (g_appResSize % iconRow == 0 ? 0 : 1);  // 计算行数
    this->setColumnCount(columnCount);  // 设置行数

    // 遍历每一行和每一列，创建并设置图标
    for(int i = 0; i < columnCount; ++i) {
        for(int j = 0; j < iconRow; ++j) {
            int crtIndex = i * iconRow + j;  // 计算当前索引

            if(crtIndex >= g_appResSize)
                break;

            g_appRes[crtIndex].resIndex = crtIndex;  // 设置应用资源索引

            this->setCellWidget(j, i, newAppIcon(g_appRes[crtIndex]));  // 创建并设置应用图标

            this->setColumnWidth(i, itemSize.width());  // 设置行高
        }
    }
}

// 获取图标大小
int ScrollHome::GetIconSize()
{
    return iconSize;  // 返回图标大小
}

// 按钮点击时的槽函数
void ScrollHome::onIcon()
{
    QPushButton *icon = dynamic_cast<QPushButton*>(sender());  // 获取发送信号的按钮

    if(!icon)
        return;

    QWidget* widget = dynamic_cast<QWidget*>(icon->parent());  // 获取按钮的父窗口

    int index = icon->property("resIndex").toInt();  // 获取应用资源的索引

    QPoint iconPoint = widget->pos() + icon->pos();  // 计算图标的位置

    g_appRes[index].clikecdRect.setRect(iconPoint.x(), iconPoint.y(), iconSize, iconSize);  // 设置点击区域

    emit appClicked(&g_appRes[index]);  // 发送应用点击信号
}
