// Copyright 2023 Borys Boiko

#include "physicalobject.h"

#include "../math/math.h"
#include "math.h"

namespace e172 {

void e172::PhysicalObject::setMass(double mass) {
    if (e172::Math::cmpf(m_mass, mass))
        return;

    m_mass = mass;
    m_needSyncNet = true;
}

void e172::PhysicalObject::setFriction(double friction) {
    if (e172::Math::cmpf(m_friction, friction))
        return;

    m_friction = friction;
    m_needSyncNet = true;
}

void e172::PhysicalObject::blockFrictionPerTick() {
    if (m_blockFrictionPerTick)
        return;

    m_blockFrictionPerTick = true;
    m_needSyncNet = true;
}

e172::PhysicalObject::ConnectionNode e172::PhysicalObject::connectionNode(
    const Vector<double> &offset, double rotation)
{
    ConnectionNode node;
    node.m_object = this;
    node.m_offset = offset;
    node.m_rotation = rotation;
    return node;
}

void e172::PhysicalObject::resetPhysicsProperties(const e172::Vector<double> &position,
                                                  double rotation,
                                                  e172::Vector<double> velocity,
                                                  double rotationVelocity)
{
    m_positionKinematics.setValue(position);
    m_positionKinematics.setVelocity(velocity);
    m_rotationKinematics.setValue(rotation);
    m_rotationKinematics.setVelocity(rotationVelocity);
    m_needSyncNet = true;
}

e172::PhysicalObject::PhysicalObject() {
    m_rotationKinematics.setValueProcessor(&Math::constrainRadians);
}

void e172::PhysicalObject::addRotationForce(double value) {
    if (!Math::cmpf(m_mass, 0) && !Math::cmpf(value, 0)) {
        m_rotationKinematics.addAcceleration(value / m_mass);
        m_needSyncNet = true;
    }
}

void e172::PhysicalObject::addRotationGravityForce(double destiantionRotation, double coeficient) {
    const auto direction = Math::radiansDifference(destiantionRotation, rotation());
    if (direction != Math::null) {
        addRotationForce(coeficient / direction);
    }
}

void e172::PhysicalObject::addRotationPursuitForce(const e172::PhysicalObject *object,
                                                   double deltaTime)
{
    if (deltaTime != Math::null) {
        const auto direction = Math::radiansDifference(object->rotation(), rotation());
        addRotationForce((direction - rotationVelocity()) / deltaTime);
    }
}

void e172::PhysicalObject::addRotationFollowForce(double destiantionRotation,
                                                  double maxAngleDistance,
                                                  double coeficient)
{
    if (maxAngleDistance != Math::null) {
        const auto direction = Math::radiansDifference(destiantionRotation, rotation());
        const auto a = (1 - direction / maxAngleDistance);
        if (a != Math::null) {
            const auto x = (1 / a - 1);
            addRotationForce(std::abs(x) * coeficient);
        }
    }
}

void e172::PhysicalObject::addRotationRestoringForce(double destiantionRotation, double coeficient) {
    const auto direction = Math::radiansDifference(destiantionRotation, rotation());
    addRotationForce(direction * coeficient);
}

void e172::PhysicalObject::addTargetRotationForse(double destinationAngle,
                                                  double rotationForceModule,
                                                  double maxRotationVelocity)
{
    addLimitedRotationForce((static_cast<double>(
                                 Math::radiansDirection(destinationAngle, rotation()))
                                 * -2
                             + 1)
                                * rotationForceModule,
                            maxRotationVelocity);
}

void e172::PhysicalObject::addForce(const e172::Vector<double> &value)
{
    if (!Math::cmpf(m_mass, 0) && value != Vector<double>()) {
        m_positionKinematics.addAcceleration(value / m_mass);
        m_needSyncNet = true;
    }
}

void e172::PhysicalObject::addForwardForce(double module) {
    addForce(m_rotationMatrix * Vector<double>{module, 0});
}

void e172::PhysicalObject::addLeftForce(double module) {
    addForce(Vector<double>::createByAngle(module, rotation() - Math::Pi / 2));
}

void e172::PhysicalObject::addRightForce(double module) {
    addForce(Vector<double>::createByAngle(module, rotation() + Math::Pi / 2));
}

void e172::PhysicalObject::addLimitedForce(const e172::Vector<double> &value, double maxVelocity) {
    if (!Math::cmpf(m_mass, 0) && value != Vector<double>()) {
        m_positionKinematics.addLimitedAcceleration(value / m_mass, maxVelocity);
        m_needSyncNet = true;
    }
}

void e172::PhysicalObject::addLimitedForwardForce(double module, double maxVelocity) {
    addLimitedForce(Vector<double>::createByAngle(module, rotation()), maxVelocity);
}

void e172::PhysicalObject::addLimitedLeftForce(double module, double maxVelocity) {
    addLimitedForce(Vector<double>::createByAngle(module, rotation() - Math::Pi / 2), maxVelocity);
}

void e172::PhysicalObject::addLimitedRightForce(double module, double maxVelocity) {
    addLimitedForce(Vector<double>::createByAngle(module, rotation() + Math::Pi / 2), maxVelocity);
}

void e172::PhysicalObject::addLimitedRotationForce(double value, double maxAngleVelocity) {
    if (m_mass != Math::null && !Math::cmpf(value, 0)) {
        m_rotationKinematics.addLimitedAcceleration(value / m_mass, maxAngleVelocity);
        m_needSyncNet = true;
    }
}

void e172::PhysicalObject::addPursuitForce(const e172::PhysicalObject *object, double deltaTime)
{
    if (deltaTime != Math::null) {
        addForce((object->position() - position() - velocity()) / deltaTime);
    }
}

void e172::PhysicalObject::addGravityForce(const e172::Vector<double> &gravityCenter, double coeficient) {
    const auto direction = gravityCenter - position();
    const auto r2 = direction * direction;
    addForce(direction * coeficient / r2);
}

void e172::PhysicalObject::addFollowForce(const e172::Vector<double> &targetPoint,
                                          double maxDistance,
                                          double coeficient)
{
    if (maxDistance != Math::null) {
        const auto direction = targetPoint - position();
        const auto a = (1 - direction.module() / maxDistance);
        if (a != Math::null) {
            const auto x = (1 / a - 1);
            addForce(direction.normalized() * std::pow(x, 2) * coeficient);
        }
    }
}

void e172::PhysicalObject::addRestoringForce(const e172::Vector<double> &destiantionPosition, double coeficient) {
    addForce((destiantionPosition - position()) * coeficient);
}

void e172::PhysicalObject::addDistanceRelatedForce(const e172::Vector<double> &destiantionPosition,
                                                   double (*f)(double, double),
                                                   double cryticalDistance,
                                                   double coeficient)
{
    const auto direction = destiantionPosition - position();
    const auto module = direction.module();
    if (module != Math::null && cryticalDistance >= 0) {
        const auto force = f(module, cryticalDistance);
        addForce(direction / module * force * coeficient);
    }
}

void e172::PhysicalObject::addDistanceRelatedRotationForce(double destiantionAngle,
                                                           double (*f)(double, double),
                                                           double cryticalDistance,
                                                           double coeficient)
{
    const auto direction = Math::radiansDifference(destiantionAngle, rotation());
    const auto force = f(std::abs(direction), cryticalDistance);
    addRotationForce((direction >= 0 ? force : -force) * coeficient);
}

void e172::PhysicalObject::connectNodes(e172::PhysicalObject::ConnectionNode node0,
                                        e172::PhysicalObject::ConnectionNode node1,
                                        double coeficient,
                                        double rotationCoeficient)
{
    if (node0.m_object && node1.m_object) {
        const auto point0 = node0.m_object->m_rotationMatrix * node0.m_offset;
        const auto point1 = node1.m_object->m_rotationMatrix * node1.m_offset;

        node0.m_rotation = Math::constrainRadians(node0.m_rotation + Math::Pi);

        node0.m_object->addRestoringForce(node1.m_object->position() + point1 - point0, coeficient);
        node1.m_object->addRestoringForce(node0.m_object->position() + point0 - point1, coeficient);

        node0.m_object
            ->addRotationRestoringForce(Math::radiansDifference(Math::constrainRadians(
                                                                    node1.m_object->rotation()
                                                                    + node1.m_rotation),
                                                                node0.m_rotation),
                                        rotationCoeficient);
        node1.m_object
            ->addRotationRestoringForce(Math::radiansDifference(Math::constrainRadians(
                                                                    node0.m_object->rotation()
                                                                    + node0.m_rotation),
                                                                node1.m_rotation),
                                        rotationCoeficient);
    }
}

void e172::PhysicalObject::dockNodes(e172::PhysicalObject::ConnectionNode node0,
                                     e172::PhysicalObject::ConnectionNode node1,
                                     double coeficient,
                                     double rotationCoeficient)
{
    if (node0.m_object && node1.m_object) {
        const auto point0 = node0.m_object->m_rotationMatrix * node0.m_offset;
        const auto point1 = node1.m_object->m_rotationMatrix * node1.m_offset;

        node0.m_rotation = Math::constrainRadians(node0.m_rotation + Math::Pi);

        const auto &&f = [](double x, double c) {
            if (x > c) {
                return c / (x + 1 - c);
            } else {
                return x;
            }
        };

        node0.m_object->addDistanceRelatedForce(node1.m_object->position() + point1 - point0, f, 8, coeficient);
        node1.m_object->addDistanceRelatedForce(node0.m_object->position() + point0 - point1,
                                                f,
                                                8,
                                                coeficient);

        node0.m_object
            ->addDistanceRelatedRotationForce(Math::radiansDifference(Math::constrainRadians(
                                                                          node1.m_object->rotation()
                                                                          + node1.m_rotation),
                                                                      node0.m_rotation),
                                              f,
                                              1,
                                              rotationCoeficient);
        node1.m_object
            ->addDistanceRelatedRotationForce(Math::radiansDifference(Math::constrainRadians(
                                                                          node0.m_object->rotation()
                                                                          + node0.m_rotation),
                                                                      node1.m_rotation),
                                              f,
                                              1,
                                              rotationCoeficient);
    }
}

e172::PhysicalObject::Proximity e172::PhysicalObject::nodesProximity(
    const e172::PhysicalObject::ConnectionNode &node0,
    const e172::PhysicalObject::ConnectionNode &node1)
{
    return {
        (node0.position() - node1.position()).module(),
                Math::radiansDistance(node0.globalInvertedRotation(), node1.globalRotation())
    };
}

void e172::PhysicalObject::proceedPhysics(double deltaTime)
{
    if (m_mass != Math::null) {
        m_rotationKinematics.addFriction(m_friction / m_mass);
        if (!m_blockFrictionPerTick) {
            m_positionKinematics.addFriction(m_friction / m_mass);
        }
    }
    m_blockFrictionPerTick = false;

    if (!Math::cmpf(m_rotationKinematics.velocity(), 0)
        || !Math::cmpf(m_rotationKinematics.acceleration(), 0)
        || !Math::cmpf(m_positionKinematics.velocity().x(), 0)
        || !Math::cmpf(m_positionKinematics.velocity().y(), 0)
        || !Math::cmpf(m_positionKinematics.acceleration().x(), 0)
        || !Math::cmpf(m_positionKinematics.acceleration().y(), 0)) {
        m_needSyncNet = true;
    }

    m_rotationKinematics.accept(deltaTime);
    m_rotationMatrix = e172::Matrix::fromRadians(rotation());
    m_positionKinematics.accept(deltaTime);
}

Vector<double> e172::PhysicalObject::ConnectionNode::position() const
{
    if (m_object)
        return m_object->position() + (m_object->m_rotationMatrix * m_offset);

    return {};
}

Vector<double> e172::PhysicalObject::ConnectionNode::center() const
{
    if (m_object)
        return m_object->position();

    return {};
}

Vector<double> e172::PhysicalObject::ConnectionNode::rotatedOffset() const
{
    if (m_object)
        return m_object->m_rotationMatrix * m_offset;

    return {};
}

double e172::PhysicalObject::ConnectionNode::globalRotation() const
{
    if (m_object) {
        return Math::constrainRadians(m_object->rotation() + m_rotation);
    }
    return m_rotation;
}

double e172::PhysicalObject::ConnectionNode::globalInvertedRotation() const
{
    return Math::constrainRadians(globalRotation() + Math::Pi);
}

std::ostream &operator<<(std::ostream &stream, const e172::PhysicalObject::ConnectionNode &node)
{
    const auto ndr = Math::constrainRadians(node.m_rotation);
    const auto obr = (node.m_object ? node.m_object->rotation() : 0);
    return stream << "{ " << ndr << ", " << obr << ", " << Math::constrainRadians(ndr + obr) << " }";
}

} // namespace e172
