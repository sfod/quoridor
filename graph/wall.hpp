#pragma once

#include <vector>
#include <QObject>
#include <QtQml>
#include "node.hpp"


class WallEnumClass : public QObject {
    Q_OBJECT

public:
    enum WallOrientation {
        WO_Horizontal,
        WO_Vertical,
        WO_Invalid
    };
    Q_ENUMS(WallOrientation)

    static void declareQML() {
        qmlRegisterUncreatableType<WallEnumClass>("WallOrientationQML", 1, 0, "WallOrientation", "mesasge");
    }
};

class Wall {
public:
    Wall(WallEnumClass::WallOrientation wo, Node node, int len);
    ~Wall() = default;

    const std::vector<std::pair<Node, Node>> &affected_nodes() const { return affected_nodes_; }
    WallEnumClass::WallOrientation orientation() const { return orientation_; }
    const Node &node() const { return node_; }
    int len() const { return affected_nodes_.size(); }

    QString to_string() const;

private:
    WallEnumClass::WallOrientation orientation_;
    Node node_;
    std::vector<std::pair<Node, Node>> affected_nodes_;
};
