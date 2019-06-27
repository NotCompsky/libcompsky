Navigate to [the releases page](https://github.com/NotCompsky/libcompsky/releases), and download the appropriate `deb` file for your architecture (most likely amd64 for 64 bit desktop).

![Screenshot from 2019-06-26 21-48-53](https://user-images.githubusercontent.com/30552567/60247729-19290080-98b9-11e9-99d0-7147ac730188.png)

Navigate to your Downloads folder

![Screenshot from 2019-06-26 21-49-51](https://user-images.githubusercontent.com/30552567/60247738-1c23f100-98b9-11e9-9b67-01f9baa4e6dd.png)

and run

    sudo apt install /path/to/libcompsky-VERSION-ARCHITECTURE.deb

Replace `apt install` with `dpkg -i` if necessary (probably older systems).

![Screenshot from 2019-06-26 21-50-56](https://user-images.githubusercontent.com/30552567/60247750-234aff00-98b9-11e9-9d64-68cfe5bf5d94.png)

If you get an error relating to libmysqlclient/libmariadbclient not being installed, that's most likely due to the package name being different in different repositories. You can use `equivs` to circumvent this issue, but do drop a bug report to notify me so that I can look into releasing seperate `deb` files for your platform.
