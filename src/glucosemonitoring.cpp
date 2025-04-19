#include "glucosemonitoring.h"

GlucoseMonitoring::GlucoseMonitoring(QObject *parent, QChartView *graphView, QLabel *glucoseLabel, QPushButton *timeRangeButton)
    : QObject(parent), cgmGraphViewUI(graphView), glucoseLabel(glucoseLabel), timeRangeButton(timeRangeButton) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GlucoseMonitoring::generateReading);
    srand(time(nullptr));
    setupGraph();
    loadHistoricalData();
}

GlucoseMonitoring::~GlucoseMonitoring() {
    delete timer;
    delete cgmGraphView;
    saveToJson("data/glucose_data.json");
}

void GlucoseMonitoring::start() {
    timer->start(300000); // Generate reading every 5 mins
}

void GlucoseMonitoring::stop() {
    timer->stop();
}

void GlucoseMonitoring::setupGraph() {
    cgmGraphView = new QChart();
    series = new QLineSeries();
    series->setColor(Qt::white);
    cgmGraphView->legend()->hide();
    cgmGraphView->setBackgroundBrush(QBrush(QColor("black")));
    cgmGraphView->setMargins(QMargins(0, 0, 0, 0));

    // Setup X-axis (Time)
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("hh:mm");
    axisX->setTickCount(7);
    axisX->setGridLineVisible(false);

    // Setup Y-axis (Glucose mmol/L)
    QValueAxis *axisY = new QValueAxis;
    axisY->setGridLineVisible(false);
    axisY->setRange(2.2, 22.2);

    cgmGraphView->addAxis(axisX, Qt::AlignBottom);
    cgmGraphView->addAxis(axisY, Qt::AlignRight);

    cgmGraphView->addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    //threshold lines
    lowerLine = new QLineSeries();
    upperLine = new QLineSeries();

    lowerLine->setColor(Qt::red);
    upperLine->setColor(QColor(255,165,0));
    lowerLine->append(0, 0);
    upperLine->append(0, 0);

    // Create the area series between lower and upper lines
    targetArea = new QAreaSeries(upperLine, lowerLine);
    targetArea->setColor(QColor(100, 100, 100, 100));
    targetArea->setBorderColor(Qt::transparent);

    cgmGraphView->addSeries(targetArea);
    cgmGraphView->addSeries(upperLine);
    cgmGraphView->addSeries(lowerLine);

    if (cgmGraphView) {
       cgmGraphViewUI->setChart(cgmGraphView);
       cgmGraphViewUI->setOptimizationFlag(QGraphicsView::IndirectPainting, true);
       cgmGraphViewUI->setRenderHint(QPainter::Antialiasing, true);
   }

    // Initialize with default 3h range
    updateGraphRange();
}

void GlucoseMonitoring::loadHistoricalData() {
    simulatePastReadings();

    qDebug() << "Historical readings (count:" << glucoseReadings.size() << "):";
    for (const auto &reading : glucoseReadings) {
        qDebug() << "  Time:" << reading.first.toString("hh:mm:ss")
                 << "| Value:" << reading.second << "mmol/L";
    }

    for (const auto &reading : glucoseReadings) {
        series->append(reading.first.toMSecsSinceEpoch(), reading.second);
    }

    // Set initial view to show last 3 hours
    updateGraphRange();

    //update glucose label with last reading
    if (!glucoseReadings.isEmpty()) {
        double lastReading = glucoseReadings.last().second;
        updateGlucoseDisplay(lastReading);
    }
}

void GlucoseMonitoring::updateGlucoseDisplay(double value) {
    if (!glucoseLabel) return;

    QString originalStyle = glucoseLabel->styleSheet();
    QString text = QString("%1 mmol/L").arg(value, 0, 'f', 1);

    if (value <= lowAlertThreshold) {
        text = QString("LOW (%1 mmol/L)").arg(value, 0, 'f', 1);
        glucoseLabel->setStyleSheet(originalStyle + "color: red;");
    }
    else if (value >= highAlertThreshold) {
        text = QString("HIGH (%1 mmol/L)").arg(value, 0, 'f', 1);
        glucoseLabel->setStyleSheet(originalStyle + "color: orange;");
    }
    else {
        glucoseLabel->setStyleSheet(originalStyle);
    }

    glucoseLabel->setText(text);
}

void GlucoseMonitoring::cleanUpOldData() {
    QDateTime twentyFourHoursAgo = QDateTime::currentDateTime().addSecs(-24 * 3600);
    QVector<QPointF> points = series->pointsVector();
    QVector<QPointF> newPoints;

    for (const QPointF &point : points) {
        QDateTime pointTime = QDateTime::fromMSecsSinceEpoch(point.x());
        if (pointTime >= twentyFourHoursAgo) {
            newPoints.append(point);
        }
    }

    if (newPoints.size() < points.size()) {
        series->replace(newPoints);
        qDebug() << "Removed" << (points.size() - newPoints.size()) << "old points.";
    }
}

void GlucoseMonitoring::cycleTimeRange() {
    static const QVector<int> timeRanges = {1, 3, 6, 12, 24};

    int currentIndex = timeRanges.indexOf(currentTimeRange);
    if (currentIndex == -1) currentIndex = 1;

    currentIndex = (currentIndex + 1) % timeRanges.size();
    currentTimeRange = timeRanges[currentIndex];

    if (timeRangeButton) {
        timeRangeButton->setText(QString("%1h").arg(currentTimeRange));
    }

    updateGraphRange();
}

void GlucoseMonitoring::updateGraphRange() {
    QDateTime now = QDateTime::currentDateTime();
    QDateTime rangeStart = now.addSecs(-currentTimeRange * 3600);

    if (QDateTimeAxis* axisX = qobject_cast<QDateTimeAxis*>(
        cgmGraphView->axes(Qt::Horizontal).first()))
    {
        axisX->setRange(rangeStart, now);
    }

    updateThresholdLines();
}

void GlucoseMonitoring::updateThresholdLines() {
    if (!lowerLine || !upperLine) return;

    QDateTime now = QDateTime::currentDateTime();
    qint64 minTime = now.addSecs(-currentTimeRange * 3600).toMSecsSinceEpoch();
    qint64 maxTime = now.toMSecsSinceEpoch();

    lowerLine->clear();
    upperLine->clear();

    lowerLine->append(minTime, lowAlertThreshold);
    lowerLine->append(maxTime, lowAlertThreshold);

    upperLine->append(minTime, highAlertThreshold);
    upperLine->append(maxTime, highAlertThreshold);

    if (lowerLine->attachedAxes().isEmpty()) {
        lowerLine->attachAxis(cgmGraphView->axes(Qt::Horizontal).first());
        lowerLine->attachAxis(cgmGraphView->axes(Qt::Vertical).first());
    }
    if (upperLine->attachedAxes().isEmpty()) {
        upperLine->attachAxis(cgmGraphView->axes(Qt::Horizontal).first());
        upperLine->attachAxis(cgmGraphView->axes(Qt::Vertical).first());
    }
}

void GlucoseMonitoring::simulatePastReadings() {
    QDateTime currentTime = QDateTime::currentDateTime();
    QVector<QPair<QDateTime, double>> simulatedReadings;

    // Simulate readings every 5 minutes for the last 24 hours
    for (int i = 0; i < 288; ++i) {
        QDateTime timestamp = currentTime.addSecs(-(i * 300));
        double reading = calculateRealisticGlucose(timestamp);
        simulatedReadings.append(qMakePair(timestamp, reading));
    }

    glucoseReadings = simulatedReadings;

    // Emit the new readings for the graph
    for (const auto &reading : simulatedReadings) {
        emit newReading(reading.second);
    }
}

double GlucoseMonitoring::calculateRealisticGlucose(const QDateTime& timestamp) {
    QTime currentTime = timestamp.time();
    int hour = currentTime.hour();
    double baseGlucose;
    double variation;

    // Base glucose levels based on time of day
    if (hour >= 23 || hour < 5) {
        // Night time  - lower, more stable
        baseGlucose = 4.5 + (rand() % 5) / 10.0; // 4.5-5.0
        variation = (rand() % 3) / 10.0; // Small variation
    }
    else if (hour >= 5 && hour < 8) {
        // Morning  - rising
        baseGlucose = 5.0 + (rand() % 10) / 10.0; // 5.0-6.0
        variation = (rand() % 5) / 10.0; // Moderate variation
    }
    else if ((hour >= 8 && hour < 10) || (hour >= 12 && hour < 14) || (hour >= 18 && hour < 20)) {
        baseGlucose = 6.0 + (rand() % 20) / 10.0; // 6.0-8.0
        variation = (rand() % 15) / 10.0;
    }
    else {
        // Daytime between meals - moderate
        baseGlucose = 5.0 + (rand() % 10) / 10.0; // 5.0-6.0
        variation = (rand() % 7) / 10.0; // Medium variation
    }

    // Add some randomness
    double randomFactor = (rand() % 100) / 100.0; // 0.0-1.0
    double glucose = baseGlucose + (variation * randomFactor);

    double last = getLatestReading();

    // Simulate gradual hyperglycemia (5% chance during meal time)
    bool mealTime = (hour >= 8 && hour <= 10) || (hour >= 12 && hour <= 14) || (hour >= 18 && hour <= 20);
    if ((rand() % 100) < 5 && mealTime) {
        if (last > 0 && last > 7.0) {
            glucose = last + 0.5 + (rand() % 10) / 10.0;  // increase gently
        } else {
            glucose = 8.0 + (rand() % 40) / 10.0;  // 8.0–12.0
        }
        glucose = qMin(glucose, 14.0);
    }

    // Simulate post-exercise drop (10% chance in late afternoon)
    if ((rand() % 100) < 10 && (hour >= 16 && hour < 19)) {
        glucose -= 0.5 + (rand() % 10) / 10.0;  // drop by 0.5–1.5
    }

    // Simulate hypoglycemia (5% chance during sleep or after exercise)
    bool lowRiskTime = (hour >= 0 && hour < 6) || (hour >= 16 && hour < 19);
    if ((rand() % 100) < 5 && lowRiskTime) {
        if (last > 0 && last < 5.0) {
            glucose = last - 0.4 - (rand() % 10) / 10.0;  // gentle drop
        } else {
            glucose = 3.2 + (rand() % 20) / 10.0;  // 3.2–5.1
        }
        glucose = qMax(glucose, 2.8);  // don't go below 2.8
    }

    // Ensure glucose stays within safe bounds
    glucose = qBound(2.5, glucose, 15.0);

    return glucose;
}

void GlucoseMonitoring::cleanUpOldReadings() {
    QDateTime currentTime = QDateTime::currentDateTime();
    QDateTime cutoffTime = currentTime.addSecs(-currentTimeRange * 3600);
    // Remove readings older than 24 hours
    while (!glucoseReadings.isEmpty() && glucoseReadings.first().first < cutoffTime) {
        glucoseReadings.removeFirst();
    }
}

void GlucoseMonitoring::generateReading() {

    QDateTime timestamp = QDateTime::currentDateTime();
    double reading = calculateRealisticGlucose(timestamp);

    glucoseReadings.append(qMakePair(timestamp, reading));

    // Ensure proper ordering
    QVector<QPointF> points = series->pointsVector();
    points.append(QPointF(timestamp.toMSecsSinceEpoch(), reading));

    // Sort by timestamp if needed (shouldn't be necessary if data is always added in order)
    std::sort(points.begin(), points.end(),
        [](const QPointF &a, const QPointF &b) { return a.x() < b.x(); });

    series->replace(points);

    qDebug() << "Added reading at:" << timestamp.toString("hh:mm:ss")
                 << "| Total points:" << series->count()
                 << "| Last connection:"
                 << (series->count() > 1 ?
                    QDateTime::fromMSecsSinceEpoch(series->at(series->count()-2).x()).toString("hh:mm:ss")
                    : "N/A")
                 << "->"
                 << timestamp.toString("hh:mm:ss");

    cleanUpOldData();
    updateGlucoseDisplay(reading);
    cleanUpOldReadings();
    updateGraphRange();

    // Check for low or high glucose levels
    if (reading <= lowAlertThreshold) {
        qWarning() << "LOW GLUCOSE ALERT:" << reading << "mmol/L at" << timestamp.toString(Qt::ISODate);

        // Create a warning message box
        QMessageBox msgBox;
        msgBox.setWindowTitle("Low Glucose Alert");
        msgBox.setText(QString("Warning: Low Glucose Level: %1 mmol/L\nTime: %2")
                       .arg(reading, 0, 'f', 1)
                       .arg(timestamp.toString("hh:mm:ss")));

        // Apply custom stylesheet to change the background color to black
        msgBox.setStyleSheet("QMessageBox { background-color: #333; color: white; }"
                             "QLabel { color: white; }"
                             "QPushButton { background-color: #333; color: white; border: none; padding: 5px 15px; }"
                             "QPushButton:hover { background-color: #555; }");

        msgBox.exec();  // Display the message box
    }
    else if (reading >= highAlertThreshold) {
        qWarning() << "HIGH GLUCOSE ALERT:" << reading << "mmol/L at" << timestamp.toString(Qt::ISODate);

        // Create a warning message box
        QMessageBox msgBox;
        msgBox.setWindowTitle("High Glucose Alert");
        msgBox.setText(QString("Warning: High Glucose Level: %1 mmol/L\nTime: %2")
                       .arg(reading, 0, 'f', 1)
                       .arg(timestamp.toString("hh:mm:ss")));

        // Apply custom stylesheet to change the background color to black
        msgBox.setStyleSheet("QMessageBox { background-color: #333; color: white; }"
                             "QLabel { color: white; }"
                             "QPushButton { background-color: #333; color: white; border: none; padding: 5px 15px; }"
                             "QPushButton:hover { background-color: #555; }");

        msgBox.exec();  // Display the message box
    }


}

QVector<QPair<QDateTime, double>> GlucoseMonitoring::getReadings() const {
    return glucoseReadings;
}

double GlucoseMonitoring::getLatestReading() const {
    if (!glucoseReadings.isEmpty()) {
        return glucoseReadings.last().second;
    }
    return -1;
}

void GlucoseMonitoring::setAlertThresholds(double low, double high) {
    lowAlertThreshold = low;
    highAlertThreshold = high;
}

void GlucoseMonitoring::saveToJson(const QString &filename) {
    QJsonObject rootObject;

    rootObject["low_alert_threshold"] = lowAlertThreshold;
    rootObject["high_alert_threshold"] = highAlertThreshold;
    rootObject["total_readings"] = glucoseReadings.size();
    rootObject["export_timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonArray readingsArray;

    for (int i = 0; i < glucoseReadings.size(); ++i) {
        const auto& reading = glucoseReadings[i];
        QJsonObject readingObject;

        readingObject["sequence_number"] = i + 1;  // 1-based index
        readingObject["timestamp"] = reading.first.toString(Qt::ISODate);
        readingObject["value"] = reading.second;

        if (reading.second <= lowAlertThreshold) {
            readingObject["alert"] = "LOW";
        }
        else if (reading.second >= highAlertThreshold) {
            readingObject["alert"] = "HIGH";
        }
        else {
            readingObject["alert"] = "NORMAL";
        }

        readingsArray.append(readingObject);
    }

    rootObject["readings"] = readingsArray;

    QJsonDocument doc(rootObject);
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing:" << filename;
        return;
    }

    file.write(doc.toJson());
    file.close();

    // Verification
    if (QFile::exists(filename)) {
        qDebug() << "Saved" << glucoseReadings.size() << "readings to:"
                 << QFileInfo(filename).absoluteFilePath();
    } else {
        qDebug() << "Save failed!" << file.errorString();
    }
}


