#include "physicalobject.h"
#include "math.h"
#include <src/math/math.h>

double e172::PhysicalObject::mass() const {
    return m_mass;
}

void e172::PhysicalObject::setMass(double mass) {
    m_mass = mass;
}

double e172::PhysicalObject::friction() const {
    return m_friction;
}

void e172::PhysicalObject::setFriction(double friction) {
    m_friction = friction;
}

e172::Matrix e172::PhysicalObject::rotationMatrix() const
{
    return m_rotationMatrix;
}

void e172::PhysicalObject::blockFrictionPerTick() {
    m_blockFrictionPerTick = true;
}

e172::PhysicalObject::ConnectionNode e172::PhysicalObject::connectionNode(const e172::Vector &offset, double rotation) {
    ConnectionNode node;
    node.m_object = this;
    node.m_offset = offset;
    node.m_rotation = rotation;
    return node;
}

void e172::PhysicalObject::resetPhysicsProperties(e172::Vector position, double rotation, e172::Vector velocity, double rotationVelocity) {
    positionKinematics.setValue(position);
    positionKinematics.setVelocity(velocity);
    rotationKinematics.setValue(rotation);
    rotationKinematics.setVelocity(rotationVelocity);
}

e172::PhysicalObject::PhysicalObject() {
    rotationKinematics.setValueProcessor(&Math::constrainRadians);
}

void e172::PhysicalObject::addRotationForce(double value) {
    if(!Math::cmpf(m_mass, 0)) {
        rotationKinematics.addAcceleration(value / m_mass);
    }
}

void e172::PhysicalObject::addRotationGravityForce(double destiantionRotation, double coeficient) {
    const auto direction = Math::radiansDifference(destiantionRotation, rotation());
    if(direction != Math::null) {
        addRotationForce(coeficient / direction);
    }
}

void e172::PhysicalObject::addRotationPursuitForce(const e172::PhysicalObject *object, double deltaTime) {
    if(deltaTime != Math::null) {
        const auto direction = Math::radiansDifference(object->rotation(), rotation());
        addRotationForce((direction - rotationVelocity()) / deltaTime);
    }
}

void e172::PhysicalObject::addRotationFollowForce(double destiantionRotation, double maxAngleDistance, double coeficient) {
    if(maxAngleDistance != Math::null) {
        const auto direction = Math::radiansDifference(destiantionRotation, rotation());
        const auto a = (1 - direction / maxAngleDistance);
        if(a != Math::null) {
            const auto x = (1 / a - 1);
            addRotationForce(std::abs(x) * coeficient);
        }
    }
}

void e172::PhysicalObject::addRotationRestoringForce(double destiantionRotation, double coeficient) {
    const auto direction = Math::radiansDifference(destiantionRotation, rotation());
    addRotationForce(direction * coeficient);
}

void e172::PhysicalObject::addTargetRotationForse(double destinationAngle, double rotationForceModule, double maxRotationVelocity) {
    addLimitedRotationForce((static_cast<double>(Math::radiansDirection(destinationAngle, rotation())) * -2 + 1) * rotationForceModule, maxRotationVelocity);
}

void e172::PhysicalObject::addForce(const Vector& value) {
    if(!Math::cmpf(m_mass, 0)) {
        positionKinematics.addAcceleration(value / m_mass);
    }
}

void e172::PhysicalObject::addForwardForce(double module) {
    addForce(m_rotationMatrix * e172::Vector { module, 0 });
}

void e172::PhysicalObject::addLeftForce(double module) {
    addForce(Vector::createByAngle(module, rotation() - Math::Pi / 2));
}

void e172::PhysicalObject::addRightForce(double module) {
    addForce(Vector::createByAngle(module, rotation() + Math::Pi / 2));
}

void e172::PhysicalObject::addLimitedForce(const e172::Vector &value, double maxVelocity) {
    if(!Math::cmpf(m_mass, 0)) {
        positionKinematics.addLimitedAcceleration(value / m_mass, maxVelocity);
    }
}

void e172::PhysicalObject::addLimitedForwardForce(double module, double maxVelocity) {
    addLimitedForce(Vector::createByAngle(module, rotation()), maxVelocity);
}

void e172::PhysicalObject::addLimitedLeftForce(double module, double maxVelocity) {
    addLimitedForce(Vector::createByAngle(module, rotation() - Math::Pi / 2), maxVelocity);
}

void e172::PhysicalObject::addLimitedRightForce(double module, double maxVelocity) {
    addLimitedForce(Vector::createByAngle(module, rotation() + Math::Pi / 2), maxVelocity);
}

void e172::PhysicalObject::addLimitedRotationForce(double value, double maxAngleVelocity) {
    if(m_mass != Math::null) {
        rotationKinematics.addLimitedAcceleration(value / m_mass, maxAngleVelocity);
    }
}

void e172::PhysicalObject::addPursuitForce(const e172::PhysicalObject *object, double deltaTime) {
    if(deltaTime != Math::null) {
        addForce((object->position() - position() - velocity()) / deltaTime);
    }
}

void e172::PhysicalObject::addGravityForce(const e172::Vector &gravityCenter, double coeficient) {
    const auto direction = gravityCenter - position();
    const auto r2 = direction * direction;
    addForce(direction * coeficient / r2);
}

void e172::PhysicalObject::addFollowForce(const e172::Vector &targetPoint, double maxDistance, double coeficient) {
    if(maxDistance != Math::null) {
        const auto direction = targetPoint - position();
        const auto a = (1 - direction.module() / maxDistance);
        if(a != Math::null) {
            const auto x = (1 / a - 1);
            addForce(direction.normalized() * std::pow(x, 2) * coeficient);
        }
    }
}

void e172::PhysicalObject::addRestoringForce(const e172::Vector &destiantionPosition, double coeficient) {
    addForce((destiantionPosition - position()) * coeficient);
}

void e172::PhysicalObject::addDistanceRelatedForce(const e172::Vector &destiantionPosition, double (*f)(double, double), double cryticalDistance, double coeficient) {
    const auto direction = destiantionPosition - position();
    const auto module = direction.module();
    if(module != Math::null && cryticalDistance >= 0) {
        const auto force = f(module, cryticalDistance);
        addForce(direction / module * force * coeficient);
    }
}

void e172::PhysicalObject::addDistanceRelatedRotationForce(double destiantionAngle, double (*f)(double, double), double cryticalDistance, double coeficient) {
    const auto direction = Math::radiansDifference(destiantionAngle, rotation());
    const auto force = f(std::abs(direction), cryticalDistance);
    addRotationForce((direction >= 0 ? force : -force) * coeficient);
}

void e172::PhysicalObject::connectNodes(e172::PhysicalObject::ConnectionNode node0, e172::PhysicalObject::ConnectionNode node1, double coeficient, double rotationCoeficient) {
    if(node0.m_object && node1.m_object) {
        const auto point0 = node0.m_object->m_rotationMatrix * node0.m_offset;
        const auto point1 = node1.m_object->m_rotationMatrix * node1.m_offset;

        node0.m_rotation = Math::constrainRadians(node0.m_rotation + Math::Pi);

        node0.m_object->addRestoringForce(node1.m_object->position() + point1 - point0, coeficient);
        node1.m_object->addRestoringForce(node0.m_object->position() + point0 - point1, coeficient);

        node0.m_object->addRotationRestoringForce(Math::radiansDifference(Math::constrainRadians(node1.m_object->rotation() + node1.m_rotation), node0.m_rotation), rotationCoeficient);
        node1.m_object->addRotationRestoringForce(Math::radiansDifference(Math::constrainRadians(node0.m_object->rotation() + node0.m_rotation), node1.m_rotation), rotationCoeficient);
    }
}

void e172::PhysicalObject::dockNodes(e172::PhysicalObject::ConnectionNode node0, e172::PhysicalObject::ConnectionNode node1, double coeficient, double rotationCoeficient) {
    if(node0.m_object && node1.m_object) {
        const auto point0 = node0.m_object->m_rotationMatrix * node0.m_offset;
        const auto point1 = node1.m_object->m_rotationMatrix * node1.m_offset;

        node0.m_rotation = Math::constrainRadians(node0.m_rotation + Math::Pi);

        const auto f = [](double x, double c) {
            if(x > c) {
                return c / (x + 1 - c);
            } else {
                return x;
            }
        };

        node0.m_object->addDistanceRelatedForce(node1.m_object->position() + point1 - point0, f, 8, coeficient);
        node1.m_object->addDistanceRelatedForce(node0.m_object->position() + point0 - point1, f, 8, coeficient);

        node0.m_object->addDistanceRelatedRotationForce(Math::radiansDifference(Math::constrainRadians(node1.m_object->rotation() + node1.m_rotation), node0.m_rotation), f, 1, rotationCoeficient);
        node1.m_object->addDistanceRelatedRotationForce(Math::radiansDifference(Math::constrainRadians(node0.m_object->rotation() + node0.m_rotation), node1.m_rotation), f, 1, rotationCoeficient);
    }
}

e172::PhysicalObject::Proximity e172::PhysicalObject::nodesProximity(const e172::PhysicalObject::ConnectionNode &node0, const e172::PhysicalObject::ConnectionNode &node1) {
    return {
        (node0.position() - node1.position()).module(),
                Math::radiansDistance(node0.globalInvertedRotation(), node1.globalRotation())
    };
}

void e172::PhysicalObject::proceedPhysics(double deltaTime) {
    if(m_mass != Math::null) {
        rotationKinematics.addFriction(m_friction / m_mass);
        if(!m_blockFrictionPerTick) {
            positionKinematics.addFriction(m_friction / m_mass);
        }
    }
    m_blockFrictionPerTick = false;

    rotationKinematics.accept(deltaTime);
    m_rotationMatrix = e172::Matrix::fromRadians(rotation());
    positionKinematics.accept(deltaTime);

}


e172::Vector e172::PhysicalObject::ConnectionNode::position() const {
    if(m_object)
        return m_object->position() + (m_object->m_rotationMatrix * m_offset);

    return e172::Vector();
}

e172::Vector e172::PhysicalObject::ConnectionNode::center() const {
    if(m_object)
        return m_object->position();

    return e172::Vector();
}

e172::Vector e172::PhysicalObject::ConnectionNode::rotatedOffset() const {
    if(m_object)
        return m_object->m_rotationMatrix * m_offset;

    return e172::Vector();
}


e172::Vector e172::PhysicalObject::ConnectionNode::offset() const {
    return m_offset;
}

double e172::PhysicalObject::ConnectionNode::globalRotation() const {
    if(m_object) {
        return Math::constrainRadians(m_object->rotation() + m_rotation);
    }
    return m_rotation;
}

double e172::PhysicalObject::ConnectionNode::globalInvertedRotation() const {
    return Math::constrainRadians(globalRotation() + Math::Pi);
}

double e172::PhysicalObject::ConnectionNode::rotation() const {
    return m_rotation;
}

namespace e172 {


std::ostream &operator<<(std::ostream &stream, const e172::PhysicalObject::ConnectionNode &node) {
    const auto ndr = Math::constrainRadians(node.m_rotation);
    const auto obr = (node.m_object ? node.m_object->rotation() : 0);
    return stream << "{ " << ndr << ", " << obr << ", " << Math::constrainRadians(ndr + obr) << " }";
}
}
