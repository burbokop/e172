#pragma once

#include <src/math/kinematics.h>
#include <src/math/matrix.h>
#include <src/math/vector.h>

namespace e172 {

class PhysicalObject
{
public:
    PhysicalObject();

    class ConnectionNode
    {
        friend PhysicalObject;
    public:
        e172::Vector position() const;
        e172::Vector center() const;
        e172::Vector rotatedOffset() const;
        Vector offset() const;
        friend std::ostream &operator<<(std::ostream& stream, const ConnectionNode& node);
        double rotation() const;
        double globalRotation() const;
        double globalInvertedRotation() const;

    private:
        PhysicalObject *m_object = nullptr;
        Vector m_offset;
        double m_rotation;
    };

    ConnectionNode connectionNode(const Vector &offset, double rotation);

    void resetPhysicsProperties(e172::Vector position, double rotation, e172::Vector velocity = e172::Vector(), double rotationVelocity = 0);

    auto rotation() const { return m_rotationKinematics.value(); };
    auto position() const { return m_positionKinematics.value(); };

    auto rotationVelocity() const { return m_rotationKinematics.velocity(); };
    auto velocity() const { return m_positionKinematics.velocity(); };

    auto rotationAcceleration() const { return m_rotationKinematics.acceleration(); };
    auto acceleration() const { return m_positionKinematics.acceleration(); };

    void addRotationForce(double value);

    void addRotationGravityForce(double destiantionRotation, double coeficient = 1);
    void addRotationPursuitForce(const PhysicalObject *object, double deltaTime);
    void addRotationFollowForce(double destiantionRotation, double maxAngleDistance, double coeficient = 1);
    void addRotationRestoringForce(double destiantionRotation, double coeficient = 1);

    void addTargetRotationForse(double destinationAngle, double rotationForceModule, double maxRotationVelocity);

    void addForce(const Vector& value);
    void addForwardForce(double module);
    void addLeftForce(double module);
    void addRightForce(double module);

    void addLimitedRotationForce(double value, double maxAngleVelocity);

    void addLimitedForce(const Vector& value, double maxVelocity);
    void addLimitedForwardForce(double module, double maxVelocity);
    void addLimitedLeftForce(double module, double maxVelocity);
    void addLimitedRightForce(double module, double maxVelocity);

    void addPursuitForce(const PhysicalObject *object, double deltaTime);
    void addGravityForce(const Vector &gravityCenter, double coeficient = 1);
    void addFollowForce(const Vector &targetPoint, double maxDistance, double coeficient = 1);
    void addRestoringForce(const Vector &destiantionPosition, double coeficient = 1);

    void addDistanceRelatedForce(const Vector &destiantionPosition, double(*f)(double, double), double cryticalDistance, double coeficient = 1);
    void addDistanceRelatedRotationForce(double destiantionAngle, double(*f)(double, double), double cryticalDistance, double coeficient = 1);

    static void connectNodes(ConnectionNode node0, ConnectionNode node1, double coeficient = 1, double rotationCoeficient = 1);
    static void dockNodes(ConnectionNode node0, ConnectionNode node1, double coeficient = 1, double rotationCoeficient = 1);

    struct Proximity {
        double distance = 0;
        double angle = 0;
    };

    static Proximity nodesProximity(const ConnectionNode &node0, const ConnectionNode &node1);

    double mass() const;
    void setMass(double mass);
    double friction() const;
    void setFriction(double friction);

    void proceedPhysics(double deltaTime);

    Matrix rotationMatrix() const;
    void blockFrictionPerTick();

    bool writeNet(WriteBuffer &buf);
    bool readNet(ReadBuffer &buf);

private:
    Kinematics<double> m_rotationKinematics;
    Kinematics<Vector> m_positionKinematics;
    double m_mass = 1;
    double m_friction = 1;
    Matrix m_rotationMatrix;
    bool m_blockFrictionPerTick = false;
};

} // namespace e172
