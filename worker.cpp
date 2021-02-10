#include <QThread>

#include "worker.h"

static const char *CLASSES[] = {
    "person",
    "car",
    "motorbike",
    "bus",
    "truck",
};
const int NUM_CLASSES = sizeof(CLASSES)/sizeof(CLASSES[0]);

static const cv::Scalar COLORS[] = {
    // red  green   blue
    {0, 	255, 	255},
    {255, 	255, 	0},
    {0, 	255, 	0},
    {255, 	0, 		0}
};
const auto NUM_COLORS = sizeof(COLORS)/sizeof(COLORS[0]);

const float CONFIDENCE_THRESHOLD = 0.5f;
const float NMS_THRESHOLD = 0.8f;

const auto FONT = cv::FONT_HERSHEY_COMPLEX_SMALL;

void Worker::run()
{
    qDebug() << STR_WORKER_PROCESSING << m_id;

    typedef std::chrono::steady_clock clock;
    auto total_start = clock::now();

    std::vector<int> indices[NUM_CLASSES];
    std::vector<float> scores[NUM_CLASSES];
    std::vector<cv::Rect> boxes[NUM_CLASSES];
    std::vector<cv::Mat> detections;

    auto layers = m_net->getUnconnectedOutLayersNames();

    auto image = m_frame->toImage();
    cv::Mat frame, tmp_frame(
        image.height(),
        image.width(),
        CV_8UC4,
        const_cast<uchar*>(image.bits()),
        static_cast<size_t>(image.bytesPerLine())
    );

    cv::cvtColor(tmp_frame, frame, cv::COLOR_BGRA2BGR);
//    cv::cvtColor(tmp_frame, frame, cv::COLOR_BGRA2GRAY);

    m_net->setInput(cv::dnn::blobFromImage(
        frame, 				// input
        1/2,              	// scale factor
        cv::Size(416, 416),	// size
        cv::Scalar(),       // mean values subtracted from channels
        false,				// swap red with blue?
        false,				// crop image after resize?
        CV_32F				// depth of output blob
    ));

    auto dnn_start = clock::now();
    m_net->forward(detections, layers);
    auto dnn_end = clock::now();

    for (auto& output : detections)
    {
        const auto num_boxes = output.rows;
        for (int i = 0; i < num_boxes; i++)
        {
            auto cols = static_cast<float>(frame.cols);
            auto rows = static_cast<float>(frame.rows);
            auto x = static_cast<int>(output.at<float>(i, 0) * cols);
            auto y = static_cast<int>(output.at<float>(i, 1) * rows);
            auto w = static_cast<int>(output.at<float>(i, 2) * cols);
            auto h = static_cast<int>(output.at<float>(i, 3) * rows);
            cv::Rect rect(x - w/2, y - h/2, w, h);

            for (int c = 0; c < NUM_CLASSES; c++)
            {
                auto conf = *output.ptr<float>(i, 5 + c);
                if (conf >= CONFIDENCE_THRESHOLD)
                {
                    boxes[c].push_back(rect);
                    scores[c].push_back(conf);
                }
            }
        }
    }

    for (unsigned int c = 0; c < NUM_CLASSES; c++)
    {
        cv::dnn::NMSBoxes(boxes[c], scores[c], 0.0, NMS_THRESHOLD, indices[c]);

        for (size_t i = 0; i < indices[c].size(); ++i)
        {
            const auto color = COLORS[c % NUM_COLORS];

            auto idx = static_cast<unsigned long>(indices[c][i]);
            const auto& rect = boxes[c][idx];
            auto p1 = cv::Point(rect.x, rect.y);
            auto p2 = cv::Point(rect.x + rect.width, rect.y + rect.height);
            cv::rectangle(frame, p1, p2, color, /*thickness*/2);

            std::ostringstream label_ss;
            label_ss << CLASSES[c] << ": " << std::fixed << scores[c][idx];
            auto label = label_ss.str();
            auto clabel = label.c_str();

            int baseline;
            auto label_bg_sz = cv::getTextSize(clabel, FONT, 1, 1, &baseline);
            p1 = cv::Point(rect.x, rect.y - label_bg_sz.height - baseline - 10);
            p2 = cv::Point(rect.x + label_bg_sz.width, rect.y);
            cv::rectangle(frame, p1, p2, color, cv::FILLED);

            p1 = cv::Point(rect.x, rect.y - baseline - 5);
            cv::putText(frame, clabel, p1, FONT, 1, cv::Scalar(0, 0, 0));
        }
    }

    typedef std::chrono::milliseconds ms;
    auto total_end   = clock::now();
    auto dnn         = std::chrono::duration_cast<ms>(dnn_end   - dnn_start);
    auto detect      = std::chrono::duration_cast<ms>(dnn_end   - total_start);
    auto total       = std::chrono::duration_cast<ms>(total_end - total_start);
    auto dnn_ms      = static_cast<float>(dnn.count());
    auto detect_ms   = static_cast<float>(detect.count());
    auto total_ms    = static_cast<float>(total.count());
    float dnn_fps    = 1000.0f / dnn_ms;
    float detect_fps = 1000.0f / detect_ms;
    float total_fps  = 1000.0f / total_ms;

    std::ostringstream stats_ss;
    stats_ss << "dnn: "		 << dnn_ms	  << "ms, FPS: " << dnn_fps
             << ", detect: " << detect_ms << "ms, FPS: " << detect_fps
             << ", total: "  << total_ms  << "ms, FPS: " << total_fps;
    auto stats = stats_ss.str();
    auto cstats = stats.c_str();
    qDebug() << STR_WORKER << cstats;

    int baseline;
    auto stats_bg_sz = cv::getTextSize(cstats, FONT, 1, 1, &baseline);
    auto p1 = cv::Point(0, 0);
    auto p2 = cv::Point(stats_bg_sz.width, stats_bg_sz.height + 10);
    cv::rectangle(frame, p1, p2, cv::Scalar(0, 0, 0), cv::FILLED);

    p1 = cv::Point(0, stats_bg_sz.height + 5);
    cv::putText(frame, cstats, p1, FONT, 1, cv::Scalar(255, 255, 255));

    cv::imshow("asd", frame);

//    m_frame = QPixmap::fromImage(QImage(frame.data, frame.cols, frame.rows, QImage::Format_RGB888));
    m_frame->loadFromData(frame.data, frame.cols * frame.rows * 3);
    emit finished(m_id);
}
