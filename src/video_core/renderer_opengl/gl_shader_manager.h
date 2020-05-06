// Copyright 2018 yuzu Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <array>
#include <cstddef>
#include <tuple>

#include <glad/glad.h>

#include "common/common_types.h"
#include "video_core/renderer_opengl/gl_resource_manager.h"
#include "video_core/renderer_opengl/maxwell_to_gl.h"

namespace Tegra::Engines {
class Maxwell3D;
}

namespace OpenGL::GLShader {

<<<<<<< HEAD
/// Uniform structure for the Uniform Buffer Object, all vectors must be 16-byte aligned
/// @note Always keep a vec4 at the end. The GL spec is not clear whether the alignment at
///       the end of a uniform block is included in UNIFORM_BLOCK_DATA_SIZE or not.
///       Not following that rule will cause problems on some AMD drivers.
struct alignas(16) MaxwellUniformData {
    void SetFromRegs(const Tegra::Engines::Maxwell3D& maxwell);

    GLfloat y_direction;
};
static_assert(sizeof(MaxwellUniformData) == 16, "MaxwellUniformData structure size is incorrect");
static_assert(sizeof(MaxwellUniformData) < 16384,
              "MaxwellUniformData structure must be less than 16kb as per the OpenGL spec");
=======
class StageProgram final : public OGLProgram {
public:
    explicit StageProgram();
    ~StageProgram();

    void SetUniformLocations();

    void UpdateConstants();

    void SetInstanceID(GLuint instance_id) {
        state.instance_id = instance_id;
    }

    void SetFlipStage(GLuint flip_stage) {
        state.flip_stage = flip_stage;
    }

    void SetYDirection(GLfloat y_direction) {
        state.y_direction = y_direction;
    }

    void SetRescalingFactor(GLfloat rescaling_factor) {
        state.rescaling_factor = rescaling_factor;
    }

    void SetViewportScale(GLfloat x, GLfloat y) {
        state.viewport_scale = {x, y};
    }

private:
    struct State {
        union {
            std::array<GLuint, 4> config_pack{};
            struct {
                GLuint instance_id;
                GLuint flip_stage;
                GLfloat y_direction;
                GLfloat rescaling_factor;
            };
        };

        std::array<GLfloat, 2> viewport_scale{};
    };

    GLint config_pack_location = -1;
    GLint viewport_flip_location = -1;

    State state;
    State old_state;
};
>>>>>>> resolution-rescaling-4

class ProgramManager final {
public:
    explicit ProgramManager();
    ~ProgramManager();

<<<<<<< HEAD
    void Create();

    /// Updates the graphics pipeline and binds it.
    void BindGraphicsPipeline();

    /// Binds a compute shader.
    void BindComputeShader(GLuint program);

    void UseVertexShader(GLuint program) {
        current_state.vertex_shader = program;
    }

    void UseGeometryShader(GLuint program) {
        current_state.geometry_shader = program;
    }

    void UseFragmentShader(GLuint program) {
        current_state.fragment_shader = program;
    }

private:
    struct PipelineState {
        bool operator==(const PipelineState& rhs) const noexcept {
            return vertex_shader == rhs.vertex_shader && fragment_shader == rhs.fragment_shader &&
                   geometry_shader == rhs.geometry_shader;
=======
    void SetConstants(Tegra::Engines::Maxwell3D& maxwell_3d, bool rescaling);

    void ApplyTo(OpenGLState& state);

    void BindVertexShader(StageProgram* program) {
        current_state.vertex = program;
    }

    void BindGeometryShader(StageProgram* program) {
        current_state.geometry = program;
    }

    void BindFragmentShader(StageProgram* program) {
        current_state.fragment = program;
    }

private:
    struct PipelineState {
        bool operator==(const PipelineState& rhs) const {
            return vertex == rhs.vertex && fragment == rhs.fragment && geometry == rhs.geometry;
>>>>>>> resolution-rescaling-4
        }

        bool operator!=(const PipelineState& rhs) const noexcept {
            return !operator==(rhs);
        }

<<<<<<< HEAD
        GLuint vertex_shader = 0;
        GLuint fragment_shader = 0;
        GLuint geometry_shader = 0;
=======
        StageProgram* vertex{};
        StageProgram* fragment{};
        StageProgram* geometry{};
>>>>>>> resolution-rescaling-4
    };

    OGLPipeline graphics_pipeline;
    OGLPipeline compute_pipeline;
    PipelineState current_state;
    PipelineState old_state;
    bool is_graphics_bound = true;
};

} // namespace OpenGL::GLShader
