#include "EncodingTemplateRepository.hpp"
#include "utils/Logger.hpp"
#include <stdexcept>
#include <nlohmann/json.hpp> // Include JSON library for parsing

using json = nlohmann::json;

EncodingTemplateRepository::EncodingTemplateRepository(std::shared_ptr<IDatabase> db)
    : m_database(std::move(db)) {}

int EncodingTemplateRepository::createTemplate(const oatpp::Object<EncodingTemplateDTO>& encodingTemplate) const {
    // Serialize settings if not null
    std::string serializedSettings = encodingTemplate->settings ? serializeSettings(*encodingTemplate->settings.getPtr()) : "";

    const std::string query = "INSERT INTO encoding_templates (id, description, ffmpeg_command, settings) VALUES (?, ?, ?, ?) RETURNING id;";

    const int lastInsertId = m_database->executeInsertReturningId(query, {
                                                                      encodingTemplate->id ? encodingTemplate->id->c_str() : "",
                                                                      encodingTemplate->description ? encodingTemplate->description->c_str() : "",
                                                                      encodingTemplate->ffmpeg_command ? encodingTemplate->ffmpeg_command->c_str() : "",
                                                                      serializedSettings
                                                                  });

    if (lastInsertId != -1) {
        Logger::getInstance().info("Encoding template created with ID: " + std::to_string(lastInsertId));
        return lastInsertId;
    }

    Logger::getInstance().error("Failed to create encoding template.");
    throw std::runtime_error("Failed to create encoding template");
}

std::shared_ptr<EncodingTemplateDTO> EncodingTemplateRepository::getTemplateById(const std::string& templateId) const {
    const std::string query = "SELECT id, description, ffmpeg_command, settings FROM encoding_templates WHERE id = ?;";

    if (const auto result = m_database->fetchQuery(query, {templateId}); !result.empty()) {
        return mapToEncodingTemplateDto(result[0]);
    }

    Logger::getInstance().warn("Encoding template with ID " + templateId + " not found.");
    return nullptr;
}

std::vector<std::shared_ptr<EncodingTemplateDTO>> EncodingTemplateRepository::getAllTemplates() const {
    const std::string query = "SELECT id, description, ffmpeg_command, settings FROM encoding_templates;";
    const auto results = m_database->fetchQuery(query);

    std::vector<std::shared_ptr<EncodingTemplateDTO>> templates;
    templates.reserve(results.size());
    for (const auto& row : results) {
        templates.push_back(mapToEncodingTemplateDto(row));
    }

    Logger::getInstance().info("Retrieved " + std::to_string(templates.size()) + " encoding templates.");
    return templates;
}

bool EncodingTemplateRepository::updateTemplate(const std::string& templateId, const oatpp::Object<EncodingTemplateDTO>& updatedTemplate) const {
    // Check if settings is not null before serializing
    std::string serializedSettings = updatedTemplate->settings ? serializeSettings(*updatedTemplate->settings.getPtr()) : "";

    const std::string query = "UPDATE encoding_templates SET description = ?, ffmpeg_command = ?, settings = ? WHERE id = ?;";

    const bool success = m_database->executeQuery(query, {
                                                      updatedTemplate->description,
                                                      updatedTemplate->ffmpeg_command,
                                                      serializedSettings,
                                                      templateId
                                                  });

    if (success) {
        Logger::getInstance().info("Updated encoding template with ID: " + templateId);
    } else {
        Logger::getInstance().warn("Failed to update encoding template with ID: " + templateId);
    }

    return success;
}

bool EncodingTemplateRepository::deleteTemplate(const std::string& templateId) const {
    const std::string query = "DELETE FROM encoding_templates WHERE id = ?;";
    const bool success = m_database->executeQuery(query, {templateId});

    if (success) {
        Logger::getInstance().info("Deleted encoding template with ID: " + templateId);
    } else {
        Logger::getInstance().warn("Failed to delete encoding template with ID: " + templateId);
    }

    return success;
}

std::string EncodingTemplateRepository::serializeSettings(const SettingsDTO& settings) {
    json j;
    j["input"] = settings.input ? settings.input->c_str() : "";  // Convert oatpp::String to std::string or set to empty if null
    j["outputs"] = json::array();

    for (const auto& output : *settings.outputs) {
        json outputJson;
        outputJson["output"] = output->output ? output->output->c_str() : "";
        outputJson["video_codec"] = output->video_codec ? output->video_codec->c_str() : "";
        outputJson["audio_codec"] = output->audio_codec ? output->audio_codec->c_str() : "";
        outputJson["subtitle_codec"] = output->subtitle_codec ? output->subtitle_codec->c_str() : "";
        outputJson["preset"] = output->preset ? output->preset->c_str() : "";
        outputJson["rate_control_mode"] = output->rate_control_mode ? output->rate_control_mode->c_str() : "";
        outputJson["video_filter"] = output->video_filter ? output->video_filter->c_str() : "";
        outputJson["audio_filter"] = output->audio_filter ? output->audio_filter->c_str() : "";
        outputJson["subtitle_filter"] = output->subtitle_filter ? output->subtitle_filter->c_str() : "";
        outputJson["pix_fmt"] = output->pix_fmt ? output->pix_fmt->c_str() : "";
        outputJson["profile"] = output->profile ? output->profile->c_str() : "";
        outputJson["level"] = output->level ? output->level->c_str() : "";
        outputJson["color_primaries"] = output->color_primaries ? output->color_primaries->c_str() : "";
        outputJson["color_trc"] = output->color_trc ? output->color_trc->c_str() : "";
        outputJson["colorspace"] = output->colorspace ? output->colorspace->c_str() : "";
        outputJson["audio_channels"] = output->audio_channels ? static_cast<int>(*output->audio_channels) : 0;
        outputJson["audio_bitrate"] = output->audio_bitrate ? output->audio_bitrate->c_str() : "";
        outputJson["audio_language"] = output->audio_language ? output->audio_language->c_str() : "";
        outputJson["subtitle_language"] = output->subtitle_language ? output->subtitle_language->c_str() : "";

        // Serialize bitrate_settings if present
        if (output->bitrate_settings) {
            json bitrateSettingsJson;
            bitrateSettingsJson["video_bitrate"] = output->bitrate_settings->video_bitrate ? output->bitrate_settings->video_bitrate->c_str() : "";
            bitrateSettingsJson["maxrate"] = output->bitrate_settings->maxrate ? output->bitrate_settings->maxrate->c_str() : "";
            bitrateSettingsJson["minrate"] = output->bitrate_settings->minrate ? output->bitrate_settings->minrate->c_str() : "";
            bitrateSettingsJson["bufsize"] = output->bitrate_settings->bufsize ? output->bitrate_settings->bufsize->c_str() : "";
            bitrateSettingsJson["crf"] = output->bitrate_settings->crf ? *output->bitrate_settings->crf : 0.0f;
            bitrateSettingsJson["qp"] = output->bitrate_settings->qp ? *output->bitrate_settings->qp : 0.0f;
            bitrateSettingsJson["cq"] = output->bitrate_settings->cq ? *output->bitrate_settings->cq : 0.0f;
            bitrateSettingsJson["global_quality"] = output->bitrate_settings->global_quality ? *output->bitrate_settings->global_quality : 0.0f;

            outputJson["bitrate_settings"] = bitrateSettingsJson;
        }

        // Serialize advanced_settings if present
        if (output->advanced_settings) {
            json advancedSettingsJson;
            advancedSettingsJson["rc_lookahead"] = output->advanced_settings->rc_lookahead ? static_cast<int>(*output->advanced_settings->rc_lookahead) : 0;
            advancedSettingsJson["temporal_aq"] = output->advanced_settings->temporal_aq ? static_cast<int>(*output->advanced_settings->temporal_aq) : 0;
            advancedSettingsJson["spatial_aq"] = output->advanced_settings->spatial_aq ? static_cast<int>(*output->advanced_settings->spatial_aq) : 0;
            advancedSettingsJson["aq_strength"] = output->advanced_settings->aq_strength ? static_cast<int>(*output->advanced_settings->aq_strength) : 0;
            advancedSettingsJson["look_ahead"] = output->advanced_settings->look_ahead ? static_cast<int>(*output->advanced_settings->look_ahead) : 0;
            advancedSettingsJson["look_ahead_depth"] = output->advanced_settings->look_ahead_depth ? static_cast<int>(*output->advanced_settings->look_ahead_depth) : 0;
            advancedSettingsJson["x264_params"] = output->advanced_settings->x264_params ? output->advanced_settings->x264_params->c_str() : "";
            advancedSettingsJson["x265_params"] = output->advanced_settings->x265_params ? output->advanced_settings->x265_params->c_str() : "";
            advancedSettingsJson["other_params"] = output->advanced_settings->other_params ? output->advanced_settings->other_params->c_str() : "";

            outputJson["advanced_settings"] = advancedSettingsJson;
        }

        j["outputs"].push_back(outputJson);
    }

    j["filter_complex"] = settings.filter_complex ? settings.filter_complex->c_str() : "";
    j["hardware_acceleration"] = settings.hardware_acceleration ? settings.hardware_acceleration->c_str() : "";

    return j.dump();  // Convert to JSON string
}

std::shared_ptr<SettingsDTO> EncodingTemplateRepository::deserializeSettings(const std::string& serializedSettings) {
    const auto settings = SettingsDTO::createShared();
    json j = json::parse(serializedSettings);

    settings->input = j["input"].get<std::string>().c_str();

    // Deserialize each output in "outputs"
    for (const auto& outputJson : j["outputs"]) {
        auto output = EncodingDTO::createShared();
        output->output = outputJson["output"].get<std::string>().c_str();
        output->video_codec = outputJson["video_codec"].get<std::string>().c_str();
        output->audio_codec = outputJson["audio_codec"].get<std::string>().c_str();
        output->subtitle_codec = outputJson["subtitle_codec"].get<std::string>().c_str();
        output->preset = outputJson["preset"].get<std::string>().c_str();
        output->rate_control_mode = outputJson["rate_control_mode"].get<std::string>().c_str();
        output->video_filter = outputJson["video_filter"].get<std::string>().c_str();
        output->audio_filter = outputJson["audio_filter"].get<std::string>().c_str();
        output->subtitle_filter = outputJson["subtitle_filter"].get<std::string>().c_str();
        output->pix_fmt = outputJson["pix_fmt"].get<std::string>().c_str();
        output->profile = outputJson["profile"].get<std::string>().c_str();
        output->level = outputJson["level"].get<std::string>().c_str();
        output->color_primaries = outputJson["color_primaries"].get<std::string>().c_str();
        output->color_trc = outputJson["color_trc"].get<std::string>().c_str();
        output->colorspace = outputJson["colorspace"].get<std::string>().c_str();
        output->audio_channels = outputJson["audio_channels"].get<uint8_t>();
        output->audio_bitrate = outputJson["audio_bitrate"].get<std::string>().c_str();
        output->audio_language = outputJson["audio_language"].get<std::string>().c_str();
        output->subtitle_language = outputJson["subtitle_language"].get<std::string>().c_str();

        // Deserialize bitrate_settings if present
        if (outputJson.contains("bitrate_settings")) {
            const auto bitrateSettings = BitrateSettingsDTO::createShared();
            bitrateSettings->video_bitrate = outputJson["bitrate_settings"]["video_bitrate"].get<std::string>().c_str();
            bitrateSettings->maxrate = outputJson["bitrate_settings"]["maxrate"].get<std::string>().c_str();
            bitrateSettings->minrate = outputJson["bitrate_settings"]["minrate"].get<std::string>().c_str();
            bitrateSettings->bufsize = outputJson["bitrate_settings"]["bufsize"].get<std::string>().c_str();
            bitrateSettings->crf = outputJson["bitrate_settings"]["crf"].get<float>();
            bitrateSettings->qp = outputJson["bitrate_settings"]["qp"].get<float>();
            bitrateSettings->cq = outputJson["bitrate_settings"]["cq"].get<float>();
            bitrateSettings->global_quality = outputJson["bitrate_settings"]["global_quality"].get<float>();
            output->bitrate_settings = bitrateSettings;
        }

        // Deserialize advanced_settings if present
        if (outputJson.contains("advanced_settings")) {
            const auto advancedSettings = AdvancedSettingsDTO::createShared();
            advancedSettings->rc_lookahead = outputJson["advanced_settings"]["rc_lookahead"].get<uint32_t>();
            advancedSettings->temporal_aq = outputJson["advanced_settings"]["temporal_aq"].get<uint8_t>();
            advancedSettings->spatial_aq = outputJson["advanced_settings"]["spatial_aq"].get<uint8_t>();
            advancedSettings->aq_strength = outputJson["advanced_settings"]["aq_strength"].get<uint8_t>();
            advancedSettings->look_ahead = outputJson["advanced_settings"]["look_ahead"].get<uint8_t>();
            advancedSettings->look_ahead_depth = outputJson["advanced_settings"]["look_ahead_depth"].get<uint32_t>();
            advancedSettings->x264_params = outputJson["advanced_settings"]["x264_params"].get<std::string>().c_str();
            advancedSettings->x265_params = outputJson["advanced_settings"]["x265_params"].get<std::string>().c_str();
            advancedSettings->other_params = outputJson["advanced_settings"]["other_params"].get<std::string>().c_str();
            output->advanced_settings = advancedSettings;
        }

        settings->outputs->push_back(output);
    }

    settings->filter_complex = j["filter_complex"].get<std::string>().c_str();
    settings->hardware_acceleration = j["hardware_acceleration"].get<std::string>().c_str();

    // Return the Oat++ DTOWrapper as a std::shared_ptr
    return settings.getPtr();
}

std::shared_ptr<EncodingTemplateDTO> EncodingTemplateRepository::mapToEncodingTemplateDto(const std::vector<std::string>& row) {
    const auto templateDto = EncodingTemplateDTO::createShared();
    templateDto->id = row[0];
    templateDto->description = row[1];
    templateDto->ffmpeg_command = row[2];
    templateDto->settings = deserializeSettings(row[3]);

    // Use .getPtr() to return the DTOWrapper as std::shared_ptr
    return templateDto.getPtr();
}