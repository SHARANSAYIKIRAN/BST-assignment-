# Q11 — BST for Government ID Database

**Keys (insertion order):** A102, A25, A7, B100, B12, A120, B3, A45
**Comparison rule:** IDs are alphanumeric codes, so keys are compared as **strings** (lexicographic / dictionary order), not as numbers. Full C implementation: `bst_lab.c`.

---

## a) BST construction, inorder traversal, structure analysis

Inserting in the given order produces this tree:

```
A102
   \
    A25
   /   \
 A120   A7
       /   \
     A45   B100
              \
              B12
                \
                B3
```

**Inorder traversal:**
`A102, A120, A25, A45, A7, B100, B12, B3`

(This matches the sorted-string order of all 8 IDs — confirming the BST property.)

**Structure analysis:**
- Nodes = 8, **Height = 5**.
- For 8 nodes, the minimum possible height (perfectly balanced) is `⌈log₂(9)⌉ − 1 = 3`.
- The tree is **skewed**, not balanced. This happens because string comparison first splits everything by the leading letter (`A` vs `B`), so all 5 "A…" IDs land in one path and all 3 "B…" IDs get pushed deep down that same path (B100 → B12 → B3 is a strictly increasing chain, so each new B-key just extends the right spine).
- Root cause: the **insertion order** was not randomized/sorted, and prefix-heavy keys (same leading character) compare mostly on later characters, causing long chains instead of even splits.

---

## b) BST Search vs Linear Search — comparison counts

Searching for a few representative keys (`A102` = root, `B3` = deepest node, `A45` = mid-depth, `B999` = not present):

| Key  | BST comparisons | Linear comparisons | Result     |
|------|------------------|---------------------|------------|
| A102 | 1                | 1                   | found      |
| B3   | 6                | 7                   | found      |
| A45  | 4                | 8                   | found      |
| B999 | 6                | 8                   | not found  |

**Observations:**
- For a key near the root (`A102`), BST and linear search are equally cheap.
- For a key deep in the tree (`B3`), BST search still needs 6 comparisons — almost as many as scanning the whole array (7–8) — because the tree's height (5) is close to `n` (8) due to the skew.
- On a **balanced** BST of 8 nodes, worst-case search would only take `⌈log₂9⌉ = 4` comparisons, clearly beating linear search's up-to-8. Here the benefit is much smaller than expected precisely because the tree isn't balanced.
- Linear search cost grows **linearly** with `n` regardless of key; BST search cost depends on the **path length**, which is only short when the tree is balanced.

---

## c) Effect of key length / insertion order on height & performance

**Key length:** All keys here are short (2–4 chars), so string comparisons themselves are cheap (O(length) per comparison, negligible for these sizes). Key length mainly affects the *cost per comparison*, not the tree's shape — shape is governed by **relative order of the keys**, not how long they are. Longer keys would slow down each comparison slightly but wouldn't by themselves fix or worsen the skew.

**Insertion order** is the dominant factor:
- The given order (A102, A25, A7, B100, B12, A120, B3, A45) is close to worst-case for this key set: keys sharing a prefix arrive in ways that keep extending one branch (e.g., B100→B12→B3 forms a strictly increasing right-chain).
- If the same 8 keys were inserted in **sorted order**, the BST would degenerate into a **linked list of height 7** (true worst case, O(n) search).
- If inserted via **balanced/median-first insertion** (e.g., insert the sorted median first, then recursively the medians of each half — B12, A25, A45, A102, A120, B100, A7, B3, or similar), the tree would reach the **theoretical minimum height of 3**.

**Comparison with theoretical bounds:**

| Case | Height | Search worst case |
|---|---|---|
| Theoretical best (balanced) | ⌈log₂(n+1)⌉ − 1 = 3 | O(log n) = 4 comparisons |
| Theoretical worst (fully skewed, e.g. sorted insertion) | n − 1 = 7 | O(n) = 8 comparisons |
| **This tree (given order)** | **5** | up to 6 comparisons (observed) |

So the actual tree sits between the two extremes, but closer to the worst case than the best — confirming that **unordered/adversarial insertion sequences degrade a plain BST toward O(n) behavior**, even though its *average*-case (random insertion order) complexity is O(log n).

---

## Suggested approach for maintaining efficient searches as the database grows

A plain BST offers no guarantee — an unlucky (or adversarial) insertion order, like the one here, turns it into an O(n) structure. As the ID database grows, use a **self-balancing search structure** instead:

- **AVL tree** or **Red-Black tree** — rebalance automatically on insert/delete, guaranteeing O(log n) height at all times. Good when searches and updates are both frequent.
- **B-Tree / B+ Tree** — preferred for large, disk/database-backed ID stores (this is literally what real database indexes use), since it minimizes disk reads and keeps very shallow, wide trees.
- If updates are rare and the ID set is mostly static, a **sorted array with binary search** is simpler than a BST and gives guaranteed O(log n) lookups.

**Recommendation for this use case (government ID database, grows over time, needs reliable fast lookup):** use a **B+ Tree** (or a Red-Black tree if kept in-memory) rather than a plain unbalanced BST, so search performance stays O(log n) regardless of the order IDs are added in.
