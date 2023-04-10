class Node(object):
    pass

class Node(object):
    def __init__(self, val: int, next_node: Node):
        self.val = val
        self.next = next_node
        
def make_linked_list(a: list[int]) -> Node:
    prev = None
    for val in reversed(a):
        node = Node(val, prev)
        prev = node
    return prev

def print_linked_list(head: Node):
    while head:
        print(head.val, end=' -> ')
        head = head.next
    print("NULL")

def resort_linked_list(head: Node) -> Node:
    if not head:
        return None
    # walk to the end
    tail = head
    while tail.next:
        tail = tail.next
    # move nodes with negative values to after the tail
    dummy = Node(0, head)
    prev = dummy
    while head != tail:
        if head.val < 0:
            prev.next = head.next
            head.next = tail.next
            tail.next = head
            head = prev.next
        else:
            prev = head
            head = head.next
    return dummy.next

if __name__ == "__main__":
    head = make_linked_list([18, -15, 11, 8, 7, -4, 2, 1])
    print_linked_list(head)
    head = resort_linked_list(head)
    print_linked_list(head)