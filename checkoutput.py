import os

def check_output_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Check the format of the first line
    first_line = lines[0].strip().split()
    if len(first_line) != 6 or first_line[0] != "./a.out":
        print("Invalid format for the first line.")
        return False

    p, c, i, b, d = map(int, first_line[1:])
    total_producers = p * i
    total_consumers = c * i
    produced_items = set()
    consumed_items = set()
    consumed_counts = {f'consumer_{j}': 0 for j in range(c)}

    # Check the rest of the lines
    for line in lines[1:]:
        parts = line.strip().split()
        if len(parts) != 4 or parts[1] not in ("produced", "consumed"):
            print("Invalid format for line:", line)
            return False

        producer_consumer, action, item_str = parts[0], parts[1], parts[3]
        item = int(item_str)

        if producer_consumer.startswith("producer"):
            if action == "produced":
                if item in produced_items:
                    print(f"Error: Producer {producer_consumer} produced item {item} more than once.")
                    return False
                produced_items.add(item)
            else:
                print(f"Error: Producer {producer_consumer} consumed item {item}.")
                return False

        elif producer_consumer.startswith("consumer"):
            if action == "consumed":
                consumed_counts[producer_consumer] += 1
                if item in consumed_items:
                    print(f"Error: Consumer {producer_consumer} consumed item {item} more than once.")
                    return False
                consumed_items.add(item)
            else:
                print(f"Error: Consumer {producer_consumer} produced item {item}.")
                return False

        else:
            print("Invalid format for line:", line)
            return False

    # Check if all items are produced and consumed exactly once
    if not (len(produced_items) == len(consumed_items) == total_producers):
        print("Error: Some items were not produced or consumed.")
        return False
    
    for consumer, count in consumed_counts.items():
        if count != p*i/c:
            print(f"Error: {consumer} consumed {count} items, expected {p*i/c}.")
            return False

    return True

if check_output_file('output2.txt'):
    print("Successful")
else:
    print("Error")