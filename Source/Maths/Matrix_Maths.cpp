#include "Matrix_Maths.h"

#include "../Player/Camera.h"
#include "../Util/Display.h"
#include "../D_Settings.h"

namespace Maths
{
    Matrix4 createViewMatrix (const Camera& camera)
    {
        Matrix4  viewMatrix;
        viewMatrix = glm::rotate(viewMatrix, glm::radians(camera.getRotation().x), {1, 0, 0});
        viewMatrix = glm::rotate(viewMatrix, glm::radians(camera.getRotation().y), {0, 1, 0});
        viewMatrix = glm::rotate(viewMatrix, glm::radians(camera.getRotation().z), {0, 0, 1});

        return glm::translate(viewMatrix, -camera.getPosition());
    }



    Matrix4 createViewRotatationMatrix(const Vector3& rotation)
    {
        Matrix4  viewMatrix;
        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation.x), {1, 0, 0});
        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation.y), {0, 1, 0});
        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation.z), {0, 0, 1});
        return viewMatrix;
    }



    Matrix4 createModelMatrix(const Vector3& translation,
                              const Vector3& rotation,
                              const Vector3& scale)
    {
        Matrix4 transform;

        transform = glm::translate(transform, translation);

        transform = glm::rotate(transform, glm::radians(rotation.x), {1, 0, 0});
        transform = glm::rotate(transform, glm::radians(rotation.y), {0, 1, 0});
        transform = glm::rotate(transform, glm::radians(rotation.z), {1, 0, 1});

        return glm::scale(transform, {1,1,1});
    }

    Matrix4 createPerspectiveMatrix ()
    {
        return glm::perspective(glm::radians(Settings::FOV),
                                Display::getAspect(),
                                Settings::NEAR_PLANE,
                                Settings::FAR_PLANE);
    }
}
