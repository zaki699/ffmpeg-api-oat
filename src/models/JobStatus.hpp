#pragma once

#include "Job.hpp"
#include "oatpp/core/Types.hpp"

// Enum definition compatible with Oat++ DTOs
ENUM(JobStatus, v_int32,
     VALUE(PENDING, 0, "PENDING"),
     VALUE(IN_PROGRESS, 1, "IN_PROGRESS"),
     VALUE(COMPLETED, 2, "COMPLETED"),
     VALUE(FAILED, 3, "FAILED"),
     VALUE(CANCELLED, 4, "CANCELLED")
);