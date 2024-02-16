<template>
  <nav>
    <a href="#">Home</a>
    <a href="#">Try</a>
    <a href="#">Contact</a>
  </nav>

  <main>
    <h1>Lorem, ipsum dolor sit amet consectetur adipisicing elit. Atque aperiam fuga recusandae cupiditate facere voluptate, possimus veritatis asperiores minima laudantium ratione iusto excepturi blanditiis fugit esse numquam velit optio magnam.</h1>
    <div>
      <h1>Lista de Usuarios</h1>
      <ul>
        <li v-for="user in users" :key="user.id">{{ user.name }}</li>
      </ul>
    </div>
  </main>
</template>

<script>
import axios from 'axios';
  
export default {
  data() {
    return {
      users: [],
    };
  },
  mounted() {
    this.fetchUsers();
  },
  methods: {
    async fetchUsers() {
      try {
        const response = await axios.get('https://jsonplaceholder.typicode.com/users');
        this.users = response.data;
      }
      catch (error) {
        console.error('Error al obtener usuarios:', error);
      }
    },
  },
};

</script>


<style scoped>

body {
  background-color: var(--dark);
  color: white;
  font-family: sans-serif;
  margin: 0;
  padding: 0;
}

nav {
  background-color: var(--darker);
  padding: 10px;
}

nav a {
  color: var(--primary);
  text-decoration: none;
  margin-right: 15px;
}

main {
  padding-left: 30%;
  padding-right: 30%;
}
</style>
