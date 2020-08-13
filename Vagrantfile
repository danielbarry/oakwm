# Vagrantfile
#
# Setup for environment.
Vagrant.configure("2") do |config|
  # Setup the box configuration
  config.vm.box = "generic/debian9"
  config.vm.hostname = "oakwm.box"
  config.vm.network :private_network, ip: "192.168.101.101"
  config.vm.provider :virtualbox do |vb|
    # Add basic hardware values
    vb.customize [
      "modifyvm", :id,
      "--cpuexecutioncap", "25",
      "--memory", "128",
    ]
  end
  # Remove need for password
  config.ssh.insert_key = false
  # Sync directories of interest
  config.vm.synced_folder "./", "/home/vagrant/dev"
  # Setup the environment
  config.vm.provision "shell", inline: "cd /home/vagrant/dev; bash setup.sh", privileged: false
end
