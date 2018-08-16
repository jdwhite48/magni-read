#include "settingsDialog.h"

SettingsDialog::SettingsDialog() : QDialog()
{
    SettingsDialog(nullptr);
}

SettingsDialog::SettingsDialog(QWidget * parent)
    : QDialog(parent) {
    QVBoxLayout * dialogLayout = createDialogLayout();

    setLayout(dialogLayout);
    setWindowTitle(tr("Advanced Settings"));
}

/*
 * Enable/disable other guiding line settings before changing temp state
 */
void SettingsDialog::changeLineEnabled(int state) {
    switch (state) {
        case Qt::Checked :
            linePosBox->setEnabled(true);
            lineColorButton->setEnabled(true);
            lineThicknessBox->setEnabled(true);
            break;
        case Qt::Unchecked :
        default:
            linePosBox->setEnabled(false);
            lineColorButton->setEnabled(false);
            lineThicknessBox->setEnabled(false);
            break;
    }

    changeTempImageSettings();
}

/*
 * Save temporary settings for dynamically modifying image settings
 */
void SettingsDialog::changeTempImageSettings() {

    bool isLineDrawn;
    switch (guidingLineBox->checkState()) {
        case Qt::Checked:
            isLineDrawn = true;
            break;
        case Qt::Unchecked:
        default:
            isLineDrawn = false;
        break;
    }

    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");
    settings.setValue("image/tempBrightness", double(brightnessSlider->value()) );
    settings.setValue("image/tempContrast", double(contrastSlider->value()) / 100 );
    settings.setValue("image/tempColorFilter", colorFilterBox->currentText() );
    settings.setValue("image/tempAngle", rotateAngleBox->cleanText().toInt() );
    settings.setValue("controls/tempIsLineDrawn", isLineDrawn);
    settings.setValue("controls/tempLinePos", linePosBox->cleanText().toInt());
    settings.setValue("controls/tempLineThickness", lineThicknessBox->cleanText().toInt());
    settings.setValue("controls/tempLineColor", lineColorButton->getColor().name());

    emit tempSettingsChanged();
}

/*
 * Revert unsaved chantes and close dialog box
 */
void SettingsDialog::closeDialog() {

    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");
    if (settings.contains("image/brightness")) {
        settings.setValue("image/tempBrightness", settings.value("image/brightness").toDouble() );
    }

    if (settings.contains("image/contrast")) {
        settings.setValue("image/tempContrast", settings.value("image/contrast").toDouble() );
    }

    if (settings.contains("image/colorFilter")) {
        settings.setValue("image/tempColorFilter", settings.value("image/colorFilter").toString() );
    }

    if (settings.contains("image/angle")) {
        settings.setValue("image/tempAngle", settings.value("image/angle").toInt() );
    }

    if (settings.contains("controls/isLineDrawn")) {
        settings.setValue("controls/tempIsLineDrawn", settings.value("controls/isLineDrawn").toBool() );
    }

    if (settings.contains("controls/linePos")) {
        settings.setValue("controls/tempLinePos", settings.value("controls/linePos").toInt() );
    }

    if (settings.contains("controls/lineThickness")) {
        settings.setValue("controls/tempLineThickness", settings.value("controls/lineThickness").toString() );
    }

    if (settings.contains("controls/lineColor")) {
        settings.setValue("controls/tempLineColor", settings.value("controls/lineColor").toString() );
    }

    emit tempSettingsChanged();

    // Emit "rejected" signal (settings not changed) and hide window
    this->reject();
    this->close();
}

/*
 * Design dialog box that displays advanced settings & preview for the webcam view
 */
QVBoxLayout * SettingsDialog::createDialogLayout() {
    QVBoxLayout * dialogLayout = new QVBoxLayout(this);

    QGridLayout * settingsLayout = createSettingsLayout();
    QHBoxLayout * buttonLayout = createButtonLayout();

    dialogLayout->addLayout(settingsLayout);
    dialogLayout->addLayout(buttonLayout);

    return dialogLayout;
}

/*
 * Design display for settings
 */
QGridLayout * SettingsDialog::createSettingsLayout() {
    QGridLayout * settingsLayout = new QGridLayout(this);
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");

    // Create brightness and contrast sliders that vary from 1 to 100, ticks every quarter, starting in the middle.
    brightnessSlider = new QSlider(Qt::Horizontal, this);
    brightnessSlider->setTickPosition(QSlider::TicksAbove);
    // Set range from very low to very high (as if percentage)
    brightnessSlider->setMinimum(-200);
    brightnessSlider->setMaximum(200);
    brightnessSlider->setTickInterval( int((brightnessSlider->maximum() - brightnessSlider->minimum()) / 10) ); // i.e. 10%
    brightnessSlider->setSingleStep(1);
    brightnessSlider->setPageStep( int((brightnessSlider->maximum() - brightnessSlider->minimum()) / 10) );

    // Set brightness slider to previous position, or default if else
    int brightnessSliderPos = ( settings.contains("image/brightness") )
            ? int(settings.value("image/brightness").toDouble())
            : int(DEFAULT_BRIGHTNESS);
    brightnessSlider->setSliderPosition( brightnessSliderPos );

    contrastSlider = new QSlider(Qt::Horizontal, this);
    contrastSlider->setTickPosition(QSlider::TicksAbove);
    // Set range from 0.5x to 3x
    contrastSlider->setMinimum(50);
    contrastSlider->setMaximum(300);
    contrastSlider->setTickInterval(50); // i.e. tick every x0.5
    contrastSlider->setSingleStep(1);
    contrastSlider->setPageStep(50);

    // Set contrast slider to previous position, or default if else
    int contrastSliderPos = ( settings.contains("image/contrast") )
            ? int(settings.value("image/contrast").toDouble() * 100)
            : int(DEFAULT_CONTRAST * 100); // Multiply by 100 to convert from double to int scale
    contrastSlider->setSliderPosition( contrastSliderPos );

    // Drop-down list of available webcams
    webcamBox = new QComboBox(this);
    QList<QCameraInfo> webcams = QCameraInfo::availableCameras();
    QList<QString> webcamNames = {};
    if (webcams.count() > 0) {
        // Get list of webcams
        for (QCameraInfo & webcamInfo : webcams) {
            webcamNames << webcamInfo.description();
        }
    }
    webcamBox->addItems(webcamNames);

    // Spin box for color filter choice
    colorFilterBox = new QComboBox(this);
    colorFilterBox->addItem("None");
    colorFilterBox->addItem("Greyscale");
    colorFilterBox->addItem("Black and White");

    rotateAngleBox = new QSpinBox(this);
    rotateAngleBox->setRange(0, 360);
    int curAngle = (settings.contains("image/angle")) ? settings.value("image/angle").toInt() : DEFAULT_ANGLE;
    rotateAngleBox->setValue(curAngle);
    rotateAngleBox->setSingleStep(5);
    rotateAngleBox->setSuffix("°");

    // Spin box for max zoom multiplier (2x-20x, default 5x)
    maxZoomBox = new QSpinBox(this);
    maxZoomBox->setRange(2, 20);
    int curZoom = (settings.contains("image/maxZoom")) ? settings.value("image/maxZoom").toInt() : DEFAULT_ZOOM;
    maxZoomBox->setValue(curZoom);
    maxZoomBox->setSingleStep(1);
    maxZoomBox->setSuffix("x");  

    clickDragBox = new QCheckBox(this);
    bool isClickToDrag = (settings.contains("controls/clickToDrag")) ? settings.value("controls/clickToDrag").toBool() : DEFAULT_CLICK_TO_DRAG;
    clickDragBox->setCheckState( (isClickToDrag) ? Qt::Checked : Qt::Unchecked);

    // Check box whether guiding line is on or off
    guidingLineBox = new QCheckBox(this);
    bool isLineDrawn = (settings.contains("controls/isLineDrawn")) ? settings.value("controls/isLineDrawn").toBool() : DEFAULT_IS_LINE_DRAWN;
    guidingLineBox->setCheckState( (isLineDrawn) ? Qt::Checked : Qt::Unchecked);

    // Position of guiding line
    linePosBox = new QSpinBox(this);
    linePosBox->setRange(0, 100);
    int curLinePos = (settings.contains("controls/linePos")) ? settings.value("controls/linePos").toInt() : DEFAULT_LINE_POS;
    linePosBox->setValue(curLinePos);
    linePosBox->setSingleStep(5);
    linePosBox->setSuffix("%");
    if (!isLineDrawn || !guidingLineBox->isEnabled()) {
        linePosBox->setEnabled(false);
    }

    lineThicknessBox = new QSpinBox(this);
    lineThicknessBox->setRange(1, 50);
    int curLineThickness = (settings.contains("controls/lineThickness")) ? settings.value("controls/lineThickness").toInt() : DEFAULT_LINE_THICKNESS;
    lineThicknessBox->setValue(curLineThickness);
    lineThicknessBox->setSingleStep(1);
    lineThicknessBox->setSuffix(" pixels");
    if (!isLineDrawn || !guidingLineBox->isEnabled()) {
        lineThicknessBox->setEnabled(false);
    }


    // Button that chooses color of guiding line
    QString curLineColorName = (settings.contains("controls/lineColor")) ? settings.value("controls/lineColor").toString() : DEFAULT_LINE_COLOR.name();
    QColor curColor = QColor(curLineColorName);
    if (!curColor.isValid()) {
        curColor = DEFAULT_LINE_COLOR;
    }
    lineColorButton = new ColorButton(curColor, this);
    if (!isLineDrawn || !guidingLineBox->isEnabled()) {
        lineColorButton->setEnabled(false);
    }

    // Set default to previously used webcam, or to system's default webcam, or first index if else
    if (settings.contains("webcam/deviceIndex") && settings.value("webcam/deviceIndex").toInt() < webcamBox->count()) {
        webcamBox->setCurrentIndex( settings.value("webcam/deviceIndex").toInt() );
    }
    else {
        restoreWebcamDefault();
    }

    // Set default to previously used filter or to default filter.
    QString curFilter;
    if (settings.contains("image/colorFilter")) {
        curFilter = settings.value("image/colorFilter").toString();
    }
    else {
        curFilter = DEFAULT_FILTER;
    }
    int curFilterIndex = colorFilterBox->findText(curFilter);
    colorFilterBox->setCurrentIndex( curFilterIndex );

    // Construct UI layout for each row

    // Row 1: Brightness
    QLabel * brightnessLabel = new QLabel("Brightness: ", this);
    QIcon brightnessIcon = QIcon(":/media/icons/sunny.png");
    QLabel * brightnessIconLabel = new QLabel(this);
    brightnessIconLabel->setPixmap(brightnessIcon.pixmap(QSize(75, 75)));
    QLabel * minBrightnessLabel = new QLabel("0%", this);
    QLabel * maxBrightnessLabel = new QLabel("100%", this);
    settingsLayout->addWidget(brightnessLabel, 0, 0, Qt::AlignLeft);
    settingsLayout->addWidget(brightnessIconLabel, 0, 1, Qt::AlignHCenter);
    settingsLayout->addWidget(minBrightnessLabel, 0, 2, Qt::AlignHCenter);
    settingsLayout->addWidget(brightnessSlider, 0, 3, 1, 10); // Span most of the row
    settingsLayout->addWidget(maxBrightnessLabel, 0, 13, Qt::AlignHCenter);

    // Row 2: Contrast
    QLabel * contrastLabel = new QLabel("Contrast: ", this);
    QIcon contrastIcon = QIcon(":/media/icons/contrast.png");
    QLabel * contrastIconLabel = new QLabel(this);
    contrastIconLabel->setPixmap(contrastIcon.pixmap(QSize(50, 50)));
    QLabel * minContrastLabel = new QLabel("0.5x", this);
    QLabel * maxContrastLabel = new QLabel("3x", this);
    settingsLayout->addWidget(contrastLabel, 1, 0, Qt::AlignLeft);
    settingsLayout->addWidget(contrastIconLabel, 1, 1, Qt::AlignHCenter);
    settingsLayout->addWidget(minContrastLabel, 1, 2, Qt::AlignHCenter);
    settingsLayout->addWidget(contrastSlider, 1, 3, 1, 10); // Span most of the row
    settingsLayout->addWidget(maxContrastLabel, 1, 13, Qt::AlignHCenter);

    // Row 3: Webcam
    QLabel * webcamLabel = new QLabel("Webcam:", this);
    settingsLayout->addWidget(webcamLabel, 2, 0, Qt::AlignLeft);
    settingsLayout->addWidget(webcamBox, 2, 2, 1, 12); // Span the remaining part of the row

    // Row 4: Color Filter
    QLabel * filterLabel = new QLabel("Color Filter:", this);
    settingsLayout->addWidget(filterLabel, 3, 0, Qt::AlignLeft);
    settingsLayout->addWidget(colorFilterBox, 3, 2, 1, 12); // Span the remaining part of the row

    // Row 5: Rotation angle
    QLabel * angleLabel = new QLabel("Image Rotation:", this);
    settingsLayout->addWidget(angleLabel, 4, 0, Qt::AlignLeft);
    settingsLayout->addWidget(rotateAngleBox, 4, 2, 1, 12);

    // Row 6: Zoom
    QLabel * zoomLabel = new QLabel("Max Zoom:", this);
    settingsLayout->addWidget(zoomLabel, 5, 0, Qt::AlignLeft);
    settingsLayout->addWidget(maxZoomBox, 5, 2, 1, 12); // Span the remaining part of the row

    // Row 7: Click to drag
    QLabel * clickDragLabel = new QLabel("Click to Drag Image:", this);
    settingsLayout->addWidget(clickDragLabel, 6, 0, 1, 2, Qt::AlignLeft);
    settingsLayout->addWidget(clickDragBox, 6, 2, 1, 12);

    // Row 8-11: Horizontal guiding line
    QLabel * lineDrawnLabel = new QLabel("Draw Guiding Line:", this);
    settingsLayout->addWidget(lineDrawnLabel, 7, 0, 1, 2, Qt::AlignLeft);
    settingsLayout->addWidget(guidingLineBox, 7, 2, 1, 12);

    QLabel * linePosLabel = new QLabel("Guiding Line Position:", this);
    settingsLayout->addWidget(linePosLabel, 8, 0, 1, 2, Qt::AlignLeft);
    settingsLayout->addWidget(linePosBox, 8, 2, 1, 12);

    QLabel * lineColorLabel = new QLabel("Guiding Line Color:", this);
    settingsLayout->addWidget(lineColorLabel, 9, 0, 1, 2, Qt::AlignLeft);
    settingsLayout->addWidget(lineColorButton, 9, 2, 1, 12);

    QLabel * lineThicknessLabel = new QLabel("Guiding Line Thickness:", this);
    settingsLayout->addWidget(lineThicknessLabel, 10, 0, 1, 2, Qt::AlignLeft);
    settingsLayout->addWidget(lineThicknessBox, 10, 2, 1, 12);

    // Modify settings dynamically when value changes
    brightnessSlider->setTracking(true);
    contrastSlider->setTracking(true);
    connect(brightnessSlider, SIGNAL (valueChanged(int)), this, SLOT (changeTempImageSettings()), Qt::QueuedConnection );
    connect(contrastSlider, SIGNAL (valueChanged(int)), this, SLOT (changeTempImageSettings()), Qt::QueuedConnection );
    connect(colorFilterBox, SIGNAL (currentTextChanged(QString)), this, SLOT (changeTempImageSettings()), Qt::QueuedConnection );
    connect(rotateAngleBox, SIGNAL (valueChanged(int)), this, SLOT (changeTempImageSettings()), Qt::QueuedConnection );
    connect(guidingLineBox, SIGNAL (stateChanged(int)), this, SLOT (changeLineEnabled(int)), Qt::QueuedConnection );
    connect(linePosBox, SIGNAL (valueChanged(int)), this, SLOT (changeTempImageSettings()), Qt::QueuedConnection );
    connect(lineThicknessBox, SIGNAL (valueChanged(int)), this, SLOT (changeTempImageSettings()), Qt::QueuedConnection );
    connect(lineColorButton, SIGNAL (colorChanged(QColor)), this, SLOT (changeTempImageSettings()), Qt::QueuedConnection );

    return settingsLayout;
}



/*
 * Design display for buttons
 */
QHBoxLayout * SettingsDialog::createButtonLayout() {
    QHBoxLayout * buttonLayout = new QHBoxLayout(this);

    defaultButton = new QPushButton("Restore Defaults", this);
    okButton = new QPushButton("OK", this);
    cancelButton = new QPushButton("Cancel", this);

    defaultButton->setToolTip("Restore all settings to default values");
    okButton->setToolTip("Save new settings");
    cancelButton->setToolTip("Use old settings");

    // Add to layout, with "Restore Defaults" on the left, and "Cancel" and "OK" on the right
    buttonLayout->addWidget(defaultButton, 1, Qt::AlignBottom | Qt::AlignLeft);
    buttonLayout->addWidget(okButton, 1, Qt::AlignBottom | Qt::AlignRight);
    buttonLayout->addWidget(cancelButton, 0, Qt:: AlignBottom | Qt::AlignRight);

    // Perform action when buttons pressed
    connect( okButton, SIGNAL (released()), this, SLOT (saveAndCloseDialog()) );
    connect( cancelButton, SIGNAL (released()), this, SLOT (closeDialog()) );
    connect( defaultButton, SIGNAL (released()), this, SLOT (restoreDefaults()) );

    return buttonLayout;

}

/*
 * Change settings back to their default values
 */
void SettingsDialog::restoreDefaults() {
    restoreWebcamDefault();

    brightnessSlider->setSliderPosition( int(DEFAULT_BRIGHTNESS) );
    contrastSlider->setSliderPosition( int(DEFAULT_CONTRAST * 100) );
    rotateAngleBox->setValue(DEFAULT_ANGLE);
    maxZoomBox->setValue(DEFAULT_ZOOM);
    colorFilterBox->setCurrentIndex( colorFilterBox->findText(DEFAULT_FILTER) );
    clickDragBox->setCheckState( (DEFAULT_CLICK_TO_DRAG) ? Qt::Checked : Qt::Unchecked);
    guidingLineBox->setCheckState( (DEFAULT_IS_LINE_DRAWN) ? Qt::Checked : Qt::Unchecked);
    linePosBox->setValue(DEFAULT_LINE_POS);
    lineThicknessBox->setValue(DEFAULT_LINE_THICKNESS);
    lineColorButton->setColor(DEFAULT_LINE_COLOR);
}

/*
 * Find system default webcam in list (if not found, first index).
 */
void SettingsDialog::restoreWebcamDefault() {
    QList<QCameraInfo> webcams = QCameraInfo::availableCameras();

    if (!QCameraInfo::defaultCamera().isNull()) {
        for (int i = 0; i < webcams.count(); i++) {
            if (webcams[i] == QCameraInfo::defaultCamera()) {
                webcamBox->setCurrentIndex(i);
                break;
            }
        }
    }
    else {
        webcamBox->setCurrentIndex(0);
    }
}

/*
 * Emit that image settings have changed and close dialog box.
 */
void SettingsDialog::saveAndCloseDialog() {
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");

    bool isClickToDragChecked;
    switch (clickDragBox->checkState()) {
        case Qt::Checked:
            isClickToDragChecked = true;
            break;
        case Qt::Unchecked:
        default:
            isClickToDragChecked = false;
            break;
    }

    bool isLineDrawn;
    switch (guidingLineBox->checkState()) {
        case Qt::Checked:
            isLineDrawn = true;
            break;
        case Qt::Unchecked:
        default:
            isLineDrawn = false;
        break;
    }

    // Save settings in native format (Windows: registry, Other: config file)
    settings.setValue("webcam/deviceIndex", webcamBox->currentIndex());
    settings.setValue("webcam/deviceName", webcamBox->currentText());
    settings.setValue("image/brightness", double(brightnessSlider->value()));
    settings.setValue("image/contrast", double(contrastSlider->value()) / 100 ); // Divide by 100 to convert from int scale to double
    settings.setValue("image/angle", rotateAngleBox->cleanText().toInt());
    settings.setValue("image/maxZoom", maxZoomBox->cleanText().toInt());
    settings.setValue("image/colorFilter", colorFilterBox->currentText());
    settings.setValue("controls/clickToDrag", isClickToDragChecked);
    settings.setValue("controls/isLineDrawn", isLineDrawn);
    settings.setValue("controls/linePos", linePosBox->cleanText().toInt());
    settings.setValue("controls/lineThickness", lineThicknessBox->cleanText().toInt());
    settings.setValue("controls/lineColor", lineColorButton->getColor().name());


    // Emit "accepted" signal (settings changed) and hide window
    this->accept();
    this->close();
}
