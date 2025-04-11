export const path = {
  home: '/',
  user: '/user',
  chats: '/chats',
  chatDetail: '/chat-detail/:id',
  refund: '/refund',
  profile: '/user/profile',
  changePassword: '/user/password',
  historyPurchase: '/user/purchase',
  login: '/login',
  register: '/register',
  logout: '/logout',
  productDetail: ':nameId',
  cart: '/cart'
} as const;

export const purchaseUrl = 'purchases';

export const API_URL = 'https://api-shopee.onrender.com/';
