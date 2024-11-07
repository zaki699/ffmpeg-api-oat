#ifndef AWSS3PROVIDER_HPP
#define AWSS3PROVIDER_HPP

#include "IStorageProvider.hpp"
#include <aws/s3/S3Client.h>
#include <string>
#include <memory>
#include <mutex>

class AWSS3Provider : public IStorageProvider {
public:
    AWSS3Provider(const std::string& bucketName, const std::string& region, int maxRetries = 3);
    ~AWSS3Provider() override;

    bool uploadFile(const std::string& localPath, const std::string& remotePath) override;
    bool deleteFile(const std::string& remotePath) override;
    std::string getURL(const std::string& remotePath) override;

private:
    std::string m_bucketName;
    std::string m_region;
    int m_maxRetries;
    std::shared_ptr<Aws::S3::S3Client> m_s3Client;
    std::mutex m_uploadMutex;  // Mutex for thread-safe S3 operations
};

#endif // AWSS3PROVIDER_HPP