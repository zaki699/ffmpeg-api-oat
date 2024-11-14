#pragma once

#include <string>
#include <vector>
#include <optional>
#include <memory>

/**
 * @brief Interface for generic repository operations.
 * @tparam T The type of entity managed by the repository.
 */
template <typename T>
class IRepository {
public:
    virtual ~IRepository() = default;

    /**
     * @brief Retrieve an entity by its unique ID.
     * @param id The unique identifier of the entity.
     * @return An optional containing the entity if found, otherwise std::nullopt.
     */
    virtual std::optional<T> getById(int id) = 0;

    /**
     * @brief Retrieve all entities of type T.
     * @return A vector containing all entities.
     */
    virtual std::vector<T> getAll() = 0;

    /**
     * @brief Create a new entity in the repository.
     * @param entity The entity to create.
     * @return The unique ID of the newly created entity.
     * @throws std::runtime_error if the entity could not be created.
     */
    virtual int create(const T& entity) = 0;

    /**
     * @brief Update an existing entity.
     * @param id The unique identifier of the entity to update.
     * @param entity The entity data to update.
     * @return True if the update was successful; false otherwise.
     */
    virtual bool update(int id, const T& entity) = 0;

    /**
     * @brief Delete an entity by its unique ID.
     * @param id The unique identifier of the entity to delete.
     * @return True if the entity was successfully deleted; false otherwise.
     */
    virtual bool deleteById(int id) = 0;
};