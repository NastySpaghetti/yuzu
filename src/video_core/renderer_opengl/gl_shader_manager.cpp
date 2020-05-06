// Copyright 2018 yuzu Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

<<<<<<< HEAD
#include <glad/glad.h>

=======
#include <array>
>>>>>>> resolution-rescaling-4
#include "common/common_types.h"
#include "core/settings.h"
#include "video_core/engines/maxwell_3d.h"
#include "video_core/renderer_opengl/gl_shader_manager.h"

namespace OpenGL::GLShader {

<<<<<<< HEAD
ProgramManager::ProgramManager() = default;

ProgramManager::~ProgramManager() = default;

void ProgramManager::Create() {
    graphics_pipeline.Create();
    glBindProgramPipeline(graphics_pipeline.handle);
}

void ProgramManager::BindGraphicsPipeline() {
    if (!is_graphics_bound) {
        is_graphics_bound = true;
        glUseProgram(0);
    }

=======
using Maxwell = Tegra::Engines::Maxwell3D::Regs;

StageProgram::StageProgram() = default;

StageProgram::~StageProgram() = default;

void StageProgram::SetUniformLocations() {
    config_pack_location = glGetUniformLocation(handle, "config_pack");
    viewport_flip_location = glGetUniformLocation(handle, "viewport_flip");
}

void StageProgram::UpdateConstants() {
    if (state.config_pack != old_state.config_pack) {
        glProgramUniform4uiv(handle, config_pack_location, 1, state.config_pack.data());
        old_state.config_pack = state.config_pack;
    }
    if (state.viewport_scale != old_state.viewport_scale) {
        glProgramUniform2fv(handle, viewport_flip_location, 1, state.viewport_scale.data());
        old_state.viewport_scale = state.viewport_scale;
    }
}

ProgramManager::ProgramManager() {
    pipeline.Create();
}

ProgramManager::~ProgramManager() = default;

void ProgramManager::SetConstants(Tegra::Engines::Maxwell3D& maxwell_3d, bool rescaling) {
    const auto& regs = maxwell_3d.regs;
    const auto& state = maxwell_3d.state;

    // TODO(bunnei): Support more than one viewport
    const GLfloat flip_x = regs.viewport_transform[0].scale_x < 0.0 ? -1.0f : 1.0f;
    const GLfloat flip_y = regs.viewport_transform[0].scale_y < 0.0 ? -1.0f : 1.0f;

    const GLuint instance_id = state.current_instance;

    // Assign in which stage the position has to be flipped (the last stage before the fragment
    // shader).
    const GLuint flip_stage = [&]() {
        constexpr u32 geometry_index = static_cast<u32>(Maxwell::ShaderProgram::Geometry);
        if (regs.shader_config[geometry_index].enable) {
            return geometry_index;
        } else {
            return static_cast<u32>(Maxwell::ShaderProgram::VertexB);
        }
    }();

    // Y_NEGATE controls what value S2R returns for the Y_DIRECTION system value.
    const GLfloat y_direction = regs.screen_y_control.y_negate == 0 ? 1.0f : -1.0f;

    const GLfloat rescale_factor = rescaling ? Settings::values.resolution_factor : 1.0f;

    for (const auto stage :
         {current_state.vertex, current_state.geometry, current_state.fragment}) {
        if (!stage) {
            continue;
        }
        stage->SetInstanceID(instance_id);
        stage->SetFlipStage(flip_stage);
        stage->SetYDirection(y_direction);
        stage->SetViewportScale(flip_x, flip_y);
        stage->SetRescalingFactor(rescale_factor);
        stage->UpdateConstants();
    }
}

void ProgramManager::ApplyTo(OpenGLState& state) {
    UpdatePipeline();
    state.draw.shader_program = 0;
    state.draw.program_pipeline = pipeline.handle;
}

GLuint GetHandle(StageProgram* program) {
    if (!program) {
        return 0;
    }
    return program->handle;
}

void ProgramManager::UpdatePipeline() {
>>>>>>> resolution-rescaling-4
    // Avoid updating the pipeline when values have no changed
    if (old_state == current_state) {
        return;
    }

    // Workaround for AMD bug
<<<<<<< HEAD
    static constexpr GLenum all_used_stages{GL_VERTEX_SHADER_BIT | GL_GEOMETRY_SHADER_BIT |
                                            GL_FRAGMENT_SHADER_BIT};
    const GLuint handle = graphics_pipeline.handle;
    glUseProgramStages(handle, all_used_stages, 0);
    glUseProgramStages(handle, GL_VERTEX_SHADER_BIT, current_state.vertex_shader);
    glUseProgramStages(handle, GL_GEOMETRY_SHADER_BIT, current_state.geometry_shader);
    glUseProgramStages(handle, GL_FRAGMENT_SHADER_BIT, current_state.fragment_shader);
=======
    constexpr GLenum all_used_stages{GL_VERTEX_SHADER_BIT | GL_GEOMETRY_SHADER_BIT |
                                     GL_FRAGMENT_SHADER_BIT};
    glUseProgramStages(pipeline.handle, all_used_stages, 0);

    glUseProgramStages(pipeline.handle, GL_VERTEX_SHADER_BIT, GetHandle(current_state.vertex));
    glUseProgramStages(pipeline.handle, GL_GEOMETRY_SHADER_BIT, GetHandle(current_state.geometry));
    glUseProgramStages(pipeline.handle, GL_FRAGMENT_SHADER_BIT, GetHandle(current_state.fragment));
>>>>>>> resolution-rescaling-4

    old_state = current_state;
}

<<<<<<< HEAD
void ProgramManager::BindComputeShader(GLuint program) {
    is_graphics_bound = false;
    glUseProgram(program);
}

void MaxwellUniformData::SetFromRegs(const Tegra::Engines::Maxwell3D& maxwell) {
    const auto& regs = maxwell.regs;

    // Y_NEGATE controls what value S2R returns for the Y_DIRECTION system value.
    y_direction = regs.screen_y_control.y_negate == 0 ? 1.0f : -1.0f;
}

=======
>>>>>>> resolution-rescaling-4
} // namespace OpenGL::GLShader
