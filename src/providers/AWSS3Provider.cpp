#include "AWSS3Provider.hpp"
#include "Logger.hpp"
#include <aws/core/Aws.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <fstream>
#include <chrono>
#include <thread>

// Constructor
AWSS3Provider::AWSS3Provider(const std::string& bucketName, const std::string& region, int maxRetries)
    : m_bucketName(bucketName), m_region(region), m_maxRetries(maxRetries) {
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    Aws::Client::ClientConfiguration config;
    config.region = m_region;
    m_s3Client = std::make_shared<Aws::S3::S3Client>(config);

    Logger::getInstance().info("AWSS3Provider initialized.");
}

// Destructor
AWSS3Provider::~AWSS3Provider() {
    Aws::SDKOptions options;
    Aws::ShutdownAPI(options);
}

// Upload a file to S3 with retries and error handling
bool AWSS3Provider::uploadFile(const std::string& localPath, const std::string& remotePath) {
    Logger::getInstance().info("Attempting to upload file: " + localPath + " to S3 with key: " + remotePath);

    int attempt = 0;
    bool success = false;

    while (attempt < m_maxRetries) {
        attempt++;
        Logger::getInstance().info("Upload attempt " + std::to_string(attempt));

        // Open the file to upload
        std::ifstream file(localPath, std::ios::binary);
        if (!file) {
            Logger::getInstance().error("Failed to open file: " + localPath);
            return false;
        }

        // Prepare the S3 PutObject request
        Aws::S3::Model::PutObjectRequest request;
        request.SetBucket(m_bucketName.c_str());
        request.SetKey(remotePath.c_str());
        request.SetBody(std::make_shared<Aws::IOStream>(&file));

        auto outcome = m_s3Client->PutObject(request);
        if (outcome.IsSuccess()) {
            Logger::getInstance().info("File uploaded successfully to S3 with key: " + remotePath);
            success = true;
            break;
        } else {
            auto error = outcome.GetError();
            Logger::getInstance().warn("Upload attempt " + std::to_string(attempt) +
                " failed with error: " + error.GetExceptionName() + " - " + error.GetMessage());

            if (attempt < m_maxRetries) {
                std::this_thread::sleep_for(std::chrono::seconds(2 * attempt));  // Exponential backoff
            }
        }
    }

    if (!success) {
        Logger::getInstance().error("Failed to upload file to S3 after " + std::to_string(m_maxRetries) + " attempts.");
    }

    return success;
}

// Delete a file from S3
bool AWSS3Provider::deleteFile(const std::string& remotePath) {
    Logger::getInstance().info("Attempting to delete file from S3 with key: " + remotePath);

    Aws::S3::Model::DeleteObjectRequest request;
    request.SetBucket(m_bucketName.c_str());
    request.SetKey(remotePath.c_str());

    auto outcome = m_s3Client->DeleteObject(request);
    if (outcome.IsSuccess()) {
        Logger::getInstance().info("File deleted successfully from S3 with key: " + remotePath);
        return true;
    } else {
        auto error = outcome.GetError();
        Logger::getInstance().error("Failed to delete file from S3 with key: " + remotePath +
            ". Error: " + error.GetExceptionName() + " - " + error.GetMessage());
        return false;
    }
}

// Get a pre-signed URL for accessing a file on S3
std::string AWSS3Provider::getURL(const std::string& remotePath) {
    Logger::getInstance().info("Generating pre-signed URL for S3 key: " + remotePath);

    Aws::S3::Model::GetObjectRequest request;
    request.SetBucket(m_bucketName.c_str());
    request.SetKey(remotePath.c_str());

    Aws::S3::Model::GetPreSignedUrlRequest urlRequest;
    urlRequest.SetBucket(m_bucketName);
    urlRequest.SetKey(remotePath);
    urlRequest.SetExpires(std::chrono::system_clock::now() + std::chrono::hours(1));

    std::string url = m_s3Client->GeneratePresignedUrl(request.GetBucket(), request.GetKey(), Aws::Http::HttpMethod::HTTP_GET, urlRequest.GetExpires());

    if (!url.empty()) {
        Logger::getInstance().info("Generated pre-signed URL for S3 key: " + remotePath);
        return url;
    } else {
        Logger::getInstance().error("Failed to generate pre-signed URL for S3 key: " + remotePath);
        return "";
    }
}