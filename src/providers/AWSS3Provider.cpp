#include "AWSS3Provider.hpp"
#include "utils/Logger.hpp"
#include <aws/core/Aws.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <fstream>
#include <chrono>
#include <thread>

// Constructor
AWSS3Provider::AWSS3Provider(std::string  bucketName, std::string  region, const int maxRetries)
    : m_bucketName(std::move(bucketName)), m_region(std::move(region)), m_maxRetries(maxRetries) {
    const Aws::SDKOptions options;
    Aws::InitAPI(options);

    Aws::Client::ClientConfiguration config;
    config.region = m_region;
    m_s3Client = std::make_shared<Aws::S3::S3Client>(config);

    Logger::getInstance().info("AWSS3Provider initialized.");
}

// Destructor
AWSS3Provider::~AWSS3Provider() {
    const Aws::SDKOptions options;
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

        // Open the file as a shared pointer to manage its lifetime correctly.
        // Create an ifstream for the file
        auto fileStream = std::make_shared<std::ifstream>(localPath, std::ios::binary);

        if (!fileStream->is_open()) {
            throw std::runtime_error("Failed to open file at " + localPath);
        }

        // Create an S3 PutObjectRequest
        Aws::S3::Model::PutObjectRequest request;
        request.SetBucket(m_bucketName.c_str());
        request.SetKey(remotePath.c_str());



        // Cast to std::shared_ptr<Aws::IOStream> by using reinterpretation
        auto awsStream = std::shared_ptr<Aws::IOStream>(
            fileStream, reinterpret_cast<Aws::IOStream*>(fileStream.get())
        );

        request.SetBody(awsStream);

        if (auto outcome = m_s3Client->PutObject(request); outcome.IsSuccess()) {
            Logger::getInstance().info("File uploaded successfully to S3 with key: " + remotePath);
            success = true;
            break;
        } else {
            const auto& error = outcome.GetError();
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
        const auto& error = outcome.GetError();
        Logger::getInstance().error("Failed to delete file from S3 with key: " + remotePath +
            ". Error: " + error.GetExceptionName() + " - " + error.GetMessage());
        return false;
    }
}

std::string generatePresignedUrl(const Aws::String& bucketName, const Aws::String& objectKey, const int expirationInSeconds) {
    const Aws::Client::ClientConfiguration clientConfig;
    Aws::S3::S3Client s3_client(clientConfig);

    // You can specify the HTTP method (e.g., GET or PUT) based on the operation you want
    constexpr auto method = Aws::Http::HttpMethod::HTTP_GET;

    // Generate the pre-signed URL
    Aws::String presignedUrl = s3_client.GeneratePresignedUrl(bucketName, objectKey, method, expirationInSeconds);

    return presignedUrl;
}

// Get a pre-signed URL for accessing a file on S3
std::string AWSS3Provider::getURL(const std::string& remotePath) {
    Logger::getInstance().info("Generating pre-signed URL for S3 key: " + remotePath);

    // Define expiration time: current time + 1 hour
    const auto expirationTime = std::chrono::system_clock::now() + std::chrono::hours(1);

    // Convert time_point to time_t (seconds since epoch)
    const int64_t expirationTimeInt64 = std::chrono::system_clock::to_time_t(expirationTime);

    // Check if the int64_t value fits in an int range
    if (expirationTimeInt64 > std::numeric_limits<int>::max() || expirationTimeInt64 < std::numeric_limits<int>::min()) {
        throw std::runtime_error("Value exceeds int range.");
    }

    // Safely cast to int
    const int expirationTimeInt = static_cast<int>(expirationTimeInt64);

    if (std::string url = generatePresignedUrl(m_bucketName, remotePath, expirationTimeInt); !url.empty()) {
        Logger::getInstance().info("Generated pre-signed URL for S3 key: " + remotePath);
        return url;
    }
    throw std::runtime_error("Failed to generate pre-signed URL for S3 key: " + remotePath);
}
