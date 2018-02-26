import Vue from 'vue'
import Router from 'vue-router'
import SideNav from '@/components/SideNav.vue'

Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/dashboard',
      name: 'SideNav',
      component: SideNav
    }
  ]
})
