import json

# Specify the path to your JSON file
file_path = 'results.json'

# Open the JSON file and load the data
with open(file_path, 'r') as file:
    data = json.load(file)


# Store the metrics obtained by a machine from a simulation
class MachineResults:
    def __init__(self, data):
        self.avg_power_consumption = float(data['average_power_consumption'])
        self.avg_processing_time = float(data['average_processing_time'])

        self.energy_consumption = float(data['energy_consumption'])
        try:
            self.energy_efficienty = float(data['energy_efficiency'])
        except (ValueError, TypeError):
            self.energy_efficienty = 0.0

        self.forwarded_tasks = int(data['forwarded_tasks'])
        self.idleness = float(data['idleness'])

        self.processed_mflops = float(data['processed_mflops'])
        self.processed_tasks = int(data['processed_tasks'])
        self.processed_time = float(data['processed_time'])

        self.executed_node = data['simulated_on']



# Store the metrics obtained by a link from a simulation
class LinkResults:
    def __init__(self, data):
        self.down_comm_mbits = float(data["downward_communicated_mbits"])
        self.down_comm_packets = int(data["downward_communicated_packets"])
        self.down_comm_time = float(data["downward_communicated_time"])
        self.down_idleness = float(data["downward_idleness"])
        self.down_waiting_time = float(data["downward_waiting_time"])
        self.simulated_on = data["simulated_on"]
        self.up_comm_mbits = float(data["upward_communicated_mbits"])
        self.up_comm_packets = int(data["upward_communicated_packets"])
        self.up_comm_time = float(data["upward_communicated_time"])
        self.up_idleness = float(data["upward_idleness"])
        self.up_waiting_time = float(data["upward_waiting_time"])



# Store the metrics obtained by a link from a simulation
class MasterResults:
    def __init__(self, data):
      self.average_turnaround_time = float(data["average_turnaround_time"])
      self.completed_tasks = int(data["completed_tasks"])
      self.simulated_on = data["simulated_on"]
      self.total_turnaround_time = float(data["total_turnaround_time"])



def parse_services(data):
    machines = {}
    links = {}
    masters = {}

    for id, service in data['services'].items():
        if service['type'] == 'machine':
            machines[int(id)] = MachineResults(service)
        elif service['type'] == 'link':
            links[int(id)] = LinkResults(service)
        elif service['type'] == 'master':
            masters[int(id)] = MasterResults(service)

    services = {}
    services['machines'] = machines
    services['links'] = links
    services['masters'] = masters

    return services

class Results:
    def __init__(self, data):
        self.services = parse_services(data)



results = Results(data)
sorted_machines_by_key = dict(sorted(results.services['machines'].items()))

ids = list(sorted_machines_by_key.keys())
mflops = [machine.processed_mflops for machine in sorted_machines_by_key.values()]

for a in mflops:
    print(a)
# for key, machine in results.services['machines'].items():
import matplotlib.pyplot as plt

# Create a scatter plot
plt.figure(figsize=(10, 6))
plt.scatter(ids, mflops, color='blue', marker='o')

# Adding titles and labels
plt.title('Processed MFLOPS per Machine')
plt.xlabel('Machine ID')
plt.ylabel('Processed MFLOPS')

# Optionally, add a grid
plt.grid(True)

# Show the plot
plt.show()


# print(type(data['services']))
