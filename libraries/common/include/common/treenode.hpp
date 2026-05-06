#ifndef HALOGEN_LIBRARIES_COMMON_TREENODE_HPP
#define HALOGEN_LIBRARIES_COMMON_TREENODE_HPP

#include <algorithm>
#include <cassert>
#include <memory>
#include <ranges>
#include <vector>

#include "common/classmacros.hpp"

namespace halogen::common {
//------1-------2-------3-------4-------5-------6-------7-------8-------9-------0-------1-------2-------3-------//

/**
 * @brief CRTP base for a node-based tree: each node has an optional parent and owned children.
 *
 * Derive as `class MyNode : public TreeNode<MyNode> { ... };`.
 * Do not delete through the base class:
 * `TreeNode<Derived>*`.
 * Instead use `Derived*` or smart pointers to the derived type.
 *
 * Reparent by calling `detachChild()` then `addChild()`.
 */
template<typename Derived, typename Allocator = std::allocator<std::unique_ptr<Derived>>>
class TreeNode
{
public:
    HALOGEN_SMART_POINTERS(Derived);

    HALOGEN_CLASS_COPY_DELETE(TreeNode);
    HALOGEN_CLASS_MOVE_DELETE(TreeNode);

    TreeNode() = default;

    /** @brief Returns a reference to the derived type. */
    [[nodiscard]] Derived& derived() { return static_cast<Derived&>(*this); }

    /** @brief Returns a reference to the derived type. */
    [[nodiscard]] const Derived& derived() const { return static_cast<const Derived&>(*this); }

    /** @brief Returns a pointer to the parent node. */
    [[nodiscard]] Derived* parent() { return m_parent; }

    /** @brief Returns a pointer to the parent node. */
    [[nodiscard]] const Derived* parent() const { return m_parent; }

private:
    struct ChildToRef
    {
        Derived& operator()(uPtr& child) const
        {
            assert(child != nullptr && "child must not be null");
            return *child;
        }
    };

    struct ChildToConstRef
    {
        const Derived& operator()(const uPtr& child) const
        {
            assert(child != nullptr && "child must not be null");
            return *child;
        }
    };

public:
    /** @brief Returns a mutable range view of child node references. */
    [[nodiscard]] auto children()
    {
        return std::views::transform(std::ranges::ref_view(m_children), ChildToRef{});
    }

    /** @brief Returns an immutable range view of child node references. */
    [[nodiscard]] auto children() const
    {
        return std::views::transform(std::ranges::ref_view(m_children), ChildToConstRef{});
    }

    /** @brief Returns the number of child nodes. */
    [[nodiscard]] std::size_t childCount() const { return m_children.size(); }

    /** @brief Returns true if the node is a root node. */
    [[nodiscard]] bool isRoot() const { return m_parent == nullptr; }

    /**
     * @brief Takes ownership of child.
     *
     * `child->parent()` must be null (debug assert otherwise).
     * @param child The child node to add.
     */
    void addChild(uPtr child)
    {
        if (child == nullptr) return;

        assert(child->m_parent == nullptr && "call detachChild() from the old parent before calling addChild()");
        child->m_parent = static_cast<Derived*>(this);

        m_children.push_back(std::move(child));
    }

    /**
     * @brief Removes child from this node and returns ownership. Returns null if not a direct child.
     * @param child The child node to detach.
     * @return A unique pointer to the detached child node or null if the node is not a direct child.
     */
    [[nodiscard]] uPtr detachChild(Derived* child)
    {
        if (child == nullptr) return nullptr;

        auto it = findChild(child);
        if (it == m_children.end()) return nullptr;

        uPtr node = std::move(*it);
        assert(node != nullptr && "node must be valid");
        m_children.erase(it);
        node->m_parent = nullptr;
        return node;
    }

    /** @brief Clears all children from this node and resets their parent pointers. */
    void clearChildren()
    {
        resetChildParents();
        m_children.clear();
    }

protected:
    ~TreeNode()
    {
        resetChildParents();
        assert(m_parent == nullptr && "node must be detached before destruction");
    }

private:
    void resetChildParents()
    {
        std::ranges::for_each(m_children, [](auto& child) { child->m_parent = nullptr; });
    }

    typename std::vector<uPtr, Allocator>::iterator findChild(Derived* child)
    {
        return std::ranges::find_if(m_children, [child](const uPtr& p) { return p.get() == child; });
    }

    Derived* m_parent = nullptr;
    std::vector<uPtr, Allocator> m_children;
};

//------1-------2-------3-------4-------5-------6-------7-------8-------9-------0-------1-------2-------3-------//
}  // namespace halogen::common
#endif  // HALOGEN_LIBRARIES_COMMON_TREENODE_HPP
