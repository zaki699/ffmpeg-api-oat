#pragma once

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp" ///< Include Oat++ codegen macros

#include OATPP_CODEGEN_BEGIN(DTO) ///< Begin DTO codegen section

/**
 * @brief Enumeration of job statuses compatible with Oat++ DTOs.
 */
ENUM(JobStatus, v_int32,
     VALUE(PENDING, 0, "PENDING"),
     VALUE(IN_PROGRESS, 1, "IN_PROGRESS"),
     VALUE(COMPLETED, 2, "COMPLETED"),
     VALUE(FAILED, 3, "FAILED"),
     VALUE(CANCELLED, 4, "CANCELLED"),
     VALUE(UNKNOWN, 5, "UNKNOWN")
);

class JobStatusUtils {
public:
    static JobStatus fromString(const std::string& statusStr) {
        if (statusStr == "PENDING") return JobStatus::PENDING;
        if (statusStr == "COMPLETED") return JobStatus::COMPLETED;
        if (statusStr == "FAILED") return JobStatus::FAILED;
        if (statusStr == "IN_PROGRESS") return JobStatus::IN_PROGRESS;
        return JobStatus::UNKNOWN;
    }

    /**
     * @brief Converts JobStatus enum to string.
     *
     * @param status The JobStatus enum value.
     * @return String representation of the JobStatus.
     */
    static std::string toString(const JobStatus status) {
        switch (status) {
            case JobStatus::PENDING: return "PENDING";
            case JobStatus::COMPLETED: return "COMPLETED";
            case JobStatus::FAILED: return "FAILED";
            case JobStatus::IN_PROGRESS: return "IN_PROGRESS";
            default: return "UNKNOWN";
        }
    }
};

#include OATPP_CODEGEN_END(DTO) ///< End DTO codegen section