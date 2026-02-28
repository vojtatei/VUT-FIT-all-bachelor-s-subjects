/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>

void clear(GPUMemory& mem, const ClearCommand& cmd) {
  if (cmd.clearColor) {
    uint32_t bufferSize = mem.framebuffer.channels * mem.framebuffer.width * mem.framebuffer.height;
    uint8_t red = static_cast<uint8_t>(cmd.color.r * 255.f);
    uint8_t green = static_cast<uint8_t>(cmd.color.g * 255.f);
    uint8_t blue = static_cast<uint8_t>(cmd.color.b * 255.f);
    uint8_t alpha = static_cast<uint8_t>(cmd.color.a * 255.f);

    for (uint32_t i = 0; i < bufferSize; i += mem.framebuffer.channels) {
      mem.framebuffer.color[i] = red;
      mem.framebuffer.color[i + 1] = green;
      mem.framebuffer.color[i + 2] = blue;
      mem.framebuffer.color[i + 3] = alpha;
    }
  }

  if (cmd.clearDepth) {
    uint32_t bufferSize = mem.framebuffer.width * mem.framebuffer.height;
    std::fill(mem.framebuffer.depth, mem.framebuffer.depth + bufferSize, cmd.depth);
  }
}

//! [gpu_execute]
void gpu_execute(GPUMemory&mem,CommandBuffer &cb){
  (void)mem;
  (void)cb;
  /// \todo Tato funkce reprezentuje funkcionalitu grafické karty.<br>
  /// Měla by umět zpracovat command buffer, čistit framebuffer a kresli.<br>
  /// mem obsahuje paměť grafické karty.
  /// cb obsahuje command buffer pro zpracování.
  /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.


    int32_t vertexShaderInvocationCount = 0;

    for (uint32_t i = 0; i < cb.nofCommands; ++i) {

      if (cb.commands[i].type == CommandType::CLEAR) {
      const ClearCommand& clearCommand = cb.commands[i].data.clearCommand;
      clear(mem, clearCommand);
      }
      else if (cb.commands[i].type == CommandType::DRAW) {
            DrawCommand cmd = cb.commands[i].data.drawCommand;
            VertexShader vs = mem.programs[cmd.programID].vertexShader;

            for (uint32_t n = 0; n < cmd.nofVertices; ++n) {
                InVertex inVertex;
                OutVertex outVertex;
                ShaderInterface si;
                vs(outVertex, inVertex, si);
                vertexShaderInvocationCount++;
            }
        }
    }
//! [gpu_execute]
}

/**
 * @brief This function reads color from texture.
 *
 * @param texture texture
 * @param uv uv coordinates
 *
 * @return color 4 floats
 */
glm::vec4 read_texture(Texture const&texture,glm::vec2 uv){
  if(!texture.data)return glm::vec4(0.f);
  auto uv1 = glm::fract(uv);
  auto uv2 = uv1*glm::vec2(texture.width-1,texture.height-1)+0.5f;
  auto pix = glm::uvec2(uv2);
  //auto t   = glm::fract(uv2);
  glm::vec4 color = glm::vec4(0.f,0.f,0.f,1.f);
  for(uint32_t c=0;c<texture.channels;++c)
    color[c] = texture.data[(pix.y*texture.width+pix.x)*texture.channels+c]/255.f;
  return color;
}

