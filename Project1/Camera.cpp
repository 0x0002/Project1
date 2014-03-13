#include "Precompiled.h"
#include "Camera.h"

// global singleton
Camera g_camera;

/*------------------------------------------------------------------------------
initialization
------------------------------------------------------------------------------*/
void Camera::Initialize() {
    m_view = m_projection = m_largerFrustumProjection = m_viewProjection = m_invViewTransposed = MatrixIdentity();
    m_invView = m_invProjection = m_invViewProjection = MatrixIdentity();
    m_position = m_target = Vector3( 0, 0, 0 );
    m_up = Vector3( 0, 1, 0 );
}

/*------------------------------------------------------------------------------
deinitialize
------------------------------------------------------------------------------*/
void Camera::Deinitialize() {
    // do nothing
}

/*------------------------------------------------------------------------------
Recompute all matrices
------------------------------------------------------------------------------*/
void Camera::UpdateMatrices() {
    m_viewProjection = m_view * m_projection;
    m_invView = MatrixInverse( m_view );
    m_invViewTransposed = MatrixTranspose( m_invView );
    m_invProjection = MatrixInverse( m_projection );
    m_invViewProjection = MatrixInverse( m_view * m_projection );
}