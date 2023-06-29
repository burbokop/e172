// Copyright 2023 Borys Boiko

#pragma once

#include "kinematics.h"
#include "matrix.h"
#include "vector.h"

namespace e172 {

class Entity;

class PhysicalObject
{
    friend Entity;

public:
    PhysicalObject();

    class ConnectionNode
    {
        friend PhysicalObject;
    public:
        Vector<double> position() const;
        Vector<double> center() const;
        Vector<double> rotatedOffset() const;
        Vector<double> offset() const { return m_offset; }
        friend std::ostream &operator<<(std::ostream& stream, const ConnectionNode& node);
        double rotation() const { return m_rotation; }
        double globalRotation() const;
        double globalInvertedRotation() const;

    private:
        PhysicalObject *m_object = nullptr;
        Vector<double> m_offset;
        double m_rotation;
    };

    ConnectionNode connectionNode(const Vector<double> &offset, double rotation);

    void resetPhysicsProperties(const Vector<double> &position,
                                double rotation,
                                Vector<double> velocity = Vector<double>(),
                                double rotationVelocity = 0);

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

    void addForce(const Vector<double> &value);
    void addForwardForce(double module);
    void addLeftForce(double module);
    void addRightForce(double module);

    void addLimitedRotationForce(double value, double maxAngleVelocity);

    void addLimitedForce(const Vector<double> &value, double maxVelocity);
    void addLimitedForwardForce(double module, double maxVelocity);
    void addLimitedLeftForce(double module, double maxVelocity);
    void addLimitedRightForce(double module, double maxVelocity);

    void addPursuitForce(const PhysicalObject *object, double deltaTime);
    void addGravityForce(const Vector<double> &gravityCenter, double coeficient = 1);
    void addFollowForce(const Vector<double> &targetPoint,
                        double maxDistance,
                        double coeficient = 1);

    void addRestoringForce(const Vector<double> &destiantionPosition, double coeficient = 1);

    void addDistanceRelatedForce(const Vector<double> &destiantionPosition,
                                 double (*f)(double, double),
                                 double cryticalDistance,
                                 double coeficient = 1);

    void addDistanceRelatedRotationForce(double destiantionAngle,
                                         double (*f)(double, double),
                                         double cryticalDistance,
                                         double coeficient = 1);

    static void connectNodes(ConnectionNode node0,
                             ConnectionNode node1,
                             double coeficient = 1,
                             double rotationCoeficient = 1);

    static void dockNodes(ConnectionNode node0,
                          ConnectionNode node1,
                          double coeficient = 1,
                          double rotationCoeficient = 1);

    struct Proximity {
        double distance = 0;
        double angle = 0;
    };

    static Proximity nodesProximity(const ConnectionNode &node0, const ConnectionNode &node1);

    double mass() const { return m_mass; }
    void setMass(double mass);
    double friction() const { return m_friction; }
    void setFriction(double friction);

    void proceedPhysics(double deltaTime);

    Matrix rotationMatrix() const { return m_rotationMatrix; }
    void blockFrictionPerTick();

private:
    Kinematics<double> m_rotationKinematics;
    Kinematics<Vector<double>> m_positionKinematics;
    double m_mass = 1;
    double m_friction = 1;
    Matrix m_rotationMatrix = Matrix::identity();
    bool m_blockFrictionPerTick = false;
    bool m_needSyncNet = true;
};

} // namespace e172
