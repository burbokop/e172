// Copyright 2023 Borys Boiko

#pragma once

#include "matrix.h"
#include "vector.h"
#include <algorithm>
#include <utility>
#include <vector>

namespace e172 {

class Colider {
public:
    struct PositionalVector {
        Vector<double> position;
        Vector<double> vector;
        bool colided = false;

        PositionalVector leftNormal() const { return {position, vector.leftNormal()}; }
        PositionalVector rightNormal() const { return {position, vector.rightNormal()}; }

        PositionalVector operator-() const { return {position, -vector}; }
        static bool moduleLessComparator(const PositionalVector &v0, const PositionalVector &v1)
        {
            return v0.vector.module() < v1.vector.module();
        }

        Vector<double> line() const;
        static Vector<double> linesIntersection(const Vector<double> &line0,
                                                const Vector<double> &line1);
    };

    Colider() = default;

    template<typename T>
    static PositionalVector objectProjection(const T &edges, const PositionalVector &vector)
    {
        std::vector<Vector<double>> tmpProj(edges.size() * 2);
        for (size_t e = 0, count = edges.size(); e < count; ++e) {
            //tmp_proj[e * 2] = (edges[e].position - vector.position).projection(vector.vector);
            tmpProj[e * 2] = edges[e].position.projection(vector.vector);
            tmpProj[e * 2 + 1] = tmpProj[e * 2] + edges[e].vector.projection(vector.vector);
        }

        const auto it0 = std::min_element(tmpProj.begin(),
                                          tmpProj.end(),
                                          [](const Vector<double> &v0, const Vector<double> &v1) {
                                              return v0.x() < v1.x();
                                          });

        const auto it1 = std::max_element(tmpProj.begin(),
                                          tmpProj.end(),
                                          [](const Vector<double> &v0, const Vector<double> &v1) {
                                              return v0.x() < v1.x();
                                          });

        if (it0 != tmpProj.end() && it1 != tmpProj.end()) {
            return {*it0, (*it1 - *it0)};
        }
        return {};
    }
    static std::vector<PositionalVector> makeEdges(const std::vector<Vector<double>> &vertices);
    static std::vector<PositionalVector> transformed(const std::vector<PositionalVector> &vector,
                                                     const e172::Matrix &matrix);
    static Vector<double> perpendecularProjection(const Vector<double> &p0,
                                                  const Vector<double> &p1,
                                                  const Vector<double> &v);

    static bool penetration(double x0, double w0, double x1, double w1);

    std::vector<Vector<double>> vertices() const { return m_vertices; }
    void setVertices(const std::vector<Vector<double>> &vertices);
    std::vector<PositionalVector> projections() const;

    static std::pair<PositionalVector, PositionalVector> narrowCollision(Colider *c0, Colider *c1);

    const std::vector<PositionalVector> &edges() const { return m_edges; }
    void setMatrix(const Matrix &matrix) { m_matrix = matrix; }
    void setPosition(const Vector<double> &position) { m_position = position; }
    std::size_t collisionCount() const { return m_collisionCount; }
    std::size_t significantNormalCount() const { return m_significantNormalCount; }
    const std::vector<PositionalVector> &escapeVectors() const { return m_escapeVectors; }
    Vector<double> collisionPoint() const { return m_collisionPoint; }

private:
    std::vector<Vector<double>> m_vertices;
    std::vector<PositionalVector> m_edges;
    std::vector<PositionalVector> m_projections;
    std::vector<PositionalVector> m_escapeVectors;
    Vector<double> m_collisionPoint;
    Matrix m_matrix = Matrix::identity();
    Vector<double> m_position;
    std::size_t m_collisionCount = 0;
    std::size_t m_significantNormalCount = 0;
};

} // namespace e172
