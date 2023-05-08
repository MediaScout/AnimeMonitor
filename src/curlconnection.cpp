#include "curlconnection.h"
#include <QDebug>
#include <QException>

CurlConnection::CurlConnection(QObject *parent) : QObject(parent)
{
    curl_global_init(CURL_GLOBAL_ALL);
    if ((curl = curl_easy_init()) == nullptr)
        throw std::exception("Error initializing curl session");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
}

CurlConnection::~CurlConnection()
{
    if (curl != nullptr) curl_easy_cleanup(curl);
    curl_global_cleanup();
}

CurlConnection &CurlConnection::setURL(const QString &url) noexcept
{
    curl_easy_setopt(curl, CURLOPT_URL, url.toStdString().c_str());
    return *this;
}

CurlConnection &CurlConnection::followLocation(bool enable) noexcept
{
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, enable);
    return *this;
}

bool CurlConnection::readData(QByteArray &byteArray)
{
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &byteArray);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
        qDebug() << "HTTP request failed: " << curl_easy_strerror(res);
    return res == CURLE_OK;
}

size_t CurlConnection::writeCallback(char *ptr, size_t size,
                                     size_t nmemb,
                                     QByteArray *userdata)
{
    userdata->append(ptr, size * nmemb);
    return size * nmemb;
}
