<template>
    <div class="sidenav">
        {{Title}}
    </div>
</template>

<script>
export default {
  name: 'SideNav',
  data () {
    return {
      Title: 'CPPRestExemple'
    }
  },
  mounted(){
    this.Title = 'Exemplo de SideNave Carregada with Mounted.'
  }
}
</script>

<style>
.sample {
    font: 18px;
    font-family: 'Courier New', Courier, monospace
}

.sidenav .sample {
    font: 40px;
    font-family: 'Franklin Gothic Medium', 'Arial Narrow', Arial, sans-serif

}
</style>
