#ifndef CAMERA_H
#define CAMERA_H

#include "SingletonBase.h"  // singleton

/*------------------------------------------------------------------------------
Camera class. Contains view, projection and their inverse matrices
------------------------------------------------------------------------------*/
class Camera : public SingletonBase< Camera > {
public:
    // initialization
    void Initialize();

    // deinitialize
    void Deinitialize();
    
    // Update previous camera position
    void Update( float dt );

    // accessors
    inline void SetView( Vector3 const &position, Vector3 const &target, Vector3 const &up ) {
        m_position = position;
        m_target   = target;
        m_up       = up;
        m_view = MatrixLookAtRH( position, target, up );
        UpdateMatrices();
    }

    inline void SetProjection( float fov, float aspectRatio, float zNear, float zFar ) {
        m_zNear = zNear;
        m_zFar  = zFar;

        m_projection = MatrixPerspectiveFovRH( fov, aspectRatio, zNear, zFar );
        // Larger view frustum for shadows
        static const float kFrustumProjectionMultipler = 1.2f;
        m_largerFrustumProjection = MatrixPerspectiveFovRH( kFrustumProjectionMultipler * fov, aspectRatio, zNear, zFar );
        UpdateMatrices();
    }

    // Accessors
    inline float GetNear()                       const { return m_zNear;               }
    inline float GetFar()                        const { return m_zFar;                }
    inline Matrix const& GetView()               const { return m_view;                }
    inline Matrix const& GetProjection()         const { return m_projection;          }
    inline Matrix const& GetViewProjection()     const { return m_viewProjection;      }
    inline Matrix const& GetInvView()            const { return m_invView;             }
    inline Matrix const& GetInvProjection()      const { return m_invProjection;       }
    inline Matrix const& GetInvViewProjection()  const { return m_invViewProjection;   }
    inline Matrix const& GetInvViewTransposed()  const { return m_invViewTransposed;   }
    inline Vector3 const& GetPosition()          const { return m_position;            }
    inline Vector3 const& GetTarget()            const { return m_target;              }
    inline Vector3 const& GetUp()                const { return m_up;                  }

private:
    void UpdateMatrices();

private:
    Matrix m_view;
    Matrix m_projection;
    Matrix m_largerFrustumProjection;
    Matrix m_viewProjection;
    Matrix m_invView;
    Matrix m_invProjection;
    Matrix m_invViewProjection;
    Matrix m_invViewTransposed;

    Vector3 m_position;
    Vector3 m_target;
    Vector3 m_up;

    float m_zNear;
    float m_zFar;
};

// global singleton
extern Camera g_camera;

#endif