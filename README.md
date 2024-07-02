<p align="center">
  <img src="https://cdn-icons-png.flaticon.com/512/6295/6295417.png" width="100" />
</p>
<p align="center">
    <h1 align="center">C HTTP SERVER</h1>
</p>
<p align="center">
	<img src="https://img.shields.io/github/last-commit/fillipehmeireles/C-HTTP-Server?style=flat&logo=git&logoColor=white&color=0080ff" alt="last-commit">
	<img src="https://img.shields.io/github/languages/top/fillipehmeireles/C-HTTP-Server?style=flat&color=0080ff" alt="repo-top-language">
	<img src="https://img.shields.io/github/languages/count/fillipehmeireles/C-HTTP-Server?style=flat&color=0080ff" alt="repo-language-count">
<p>
<p align="center">
		<em>Developed with </em>
</p>
<p align="center">
	<img src="https://img.shields.io/badge/C-A8B9CC.svg?style=flat&logo=C&logoColor=black" alt="C">
</p>
<hr>

##  Quick Links

> - [ Overview](#-overview)
> - [ Features](#-features)
> - [ Repository Structure](#-repository-structure)
> - [ Modules](#-modules)
> - [ Getting Started](#-getting-started)
>   - [ Installation](#-installation)
>   - [ Running C-HTTP-Server](#-running-C-HTTP-Server)
>   - [ Tests](#-tests)
> - [ Project Roadmap](#-project-roadmap)
> - [ Contributing](#-contributing)
> - [ License](#-license)
> - [ Acknowledgments](#-acknowledgments)

---

##  Overview
<p>
This is a simple http server written in C.<br/>
It is a recreational project, the main idea is to learn more about the http protocol.
Futhermore, this lib was made to be as lightweight and easy to use, like NodeJS Express or Python Flask.
</p>

---


##  Features

 - Can receive requests with methods:
    - GET, POST, PATCH, PUT, DELETE, OPTIONS, HEAD
 - Implemented 404 for resource not found
 - Implemented 405 for  method not allowed
 - Implemented parameters resources: `/hello/<:id>` ([check the examples](https://github.com/fillipehmeireles/C-HTTP-Server/tree/master/examples))
 - Multithreading
   
---

##  Repository Structure

```sh
└── C-HTTP-Server/
    ├── Makefile
    ├── README.md
    ├── examples
    │   └── hello-world
    │       ├── hello_world.c
    │       └── include
    │           ├── cJSON.c
    │           └── cJSON.h
    ├── include
    │   └── server.h
    └── src
        └── server.c
```

---

##  Modules



| File                                                                                       | Summary                                      |
| ---                                                                                        | ---                                          |
| [server.h](https://github.com/fillipehmeireles/C-HTTP-Server/blob/master/include/server.h) | header file `include/server.h` |


| File                                                                                   | Summary                                  |
| ---                                                                                    | ---                                      |
| [server.c](https://github.com/fillipehmeireles/C-HTTP-Server/blob/master/src/server.c) | source code implementation `src/server.c` |


---

##  Getting Started

###  Installation
Because the entire library is only one C file and one header file, you can just copy `server.h` and `server.c` to your projects source and start using it.

###  Tests

To execute tests, run:

```sh
/* TODO */
```

---

##  Project Roadmap

- [X] `► Single thread server`
- [ ] `► Multi thread`
- [ ] `► Multiple Params resources`
- [ ] `► Query Params`
- [ ] `► Tests with Unity Lib`

---

##  Contributing

Contributions are welcome! 

1. **Fork the Repository**: Start by forking the project repository to your GitHub account.
2. **Clone Locally**: Clone the forked repository to your local machine using a Git client.
   ```sh
   git clone https://github.com/fillipehmeireles/C-HTTP-Server
   ```
3. **Create a New Branch**: Always work on a new branch, giving it a descriptive name.
   ```sh
   git checkout -b new-feature-x
   ```
4. **Make Your Changes**: Develop and test your changes locally.
5. **Commit Your Changes**: Commit with a clear message describing your updates.
   ```sh
   git commit -m 'Implemented new feature x.'
   ```
6. **Push to GitHub**: Push the changes to your forked repository.
   ```sh
   git push origin new-feature-x
   ```
7. **Submit a Pull Request**: Create a PR against the original project repository. Clearly describe the changes and their motivations.

Once your PR is reviewed and approved, it will be merged into the main branch.

---

##  License

This project is under the [MIT](https://github.com/fillipehmeireles/C-HTTP-Server/blob/master/LICENSE) License.

---

##  Acknowledgments

- In the Examples, it's being used the cJSON lib as parser. This is a very interesting lib, [please check out](https://github.com/DaveGamble/cJSON).

[**Return**](#-quick-links)

---
