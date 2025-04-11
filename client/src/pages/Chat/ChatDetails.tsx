import React, { useState } from 'react';
import { BsEmojiSmile, BsPaperclip, BsThreeDotsVertical, BsChevronLeft } from 'react-icons/bs';
import { IoMdSend } from 'react-icons/io';
import { useNavigate, useParams } from 'react-router-dom';

const ChatDetail = () => {
  const dummyChats = [
    {
      id: 1,
      name: 'Sarah Wilson',
      lastMessage: 'Hey, is the product still available?',
      timestamp: '10:30 AM',
      unread: 2,
      avatar: 'https://images.unsplash.com/photo-1494790108377-be9c29b29330',
      online: true
    },
    {
      id: 2,
      name: 'John Davis',
      lastMessage: 'Thanks for the quick response!',
      timestamp: '9:45 AM',
      unread: 0,
      avatar: 'https://images.unsplash.com/photo-1500648767791-00dcc994a43e',
      online: false
    }
  ];
  const dummyMessages = [
    {
      id: 1,
      sender: 'user',
      text: "Hi, I'm interested in your product",
      timestamp: '10:25 AM'
    },
    {
      id: 2,
      sender: 'other',
      text: 'Hello! Sure, which one are you looking at?',
      timestamp: '10:26 AM'
    },
    {
      id: 3,
      product: {
        name: 'Wireless Earbuds Pro',
        price: '$129.99',
        image: 'https://images.unsplash.com/photo-1572569511254-d8f925fe2cbb'
      }
    }
  ];
  const { id } = useParams();
  const chat = dummyChats.find((c) => c.id === Number(id));
//   const [activeChat, setActiveChat] = useState(true);
  const [messages, setMessages] = useState([]);
  const [inputMessage, setInputMessage] = useState('');
  const [showDropdown, setShowDropdown] = useState(false);
  const navigate = useNavigate()
  return (
    <div className='max-w-3xl mx-auto'>
      <div className='h-screen flex flex-col bg-white'>
        <div className='p-4 border-b flex items-center justify-between bg-white sticky top-0'>
          <div className='flex items-center space-x-4'>
            <button onClick={() => navigate('/chats')} className='text-[#333333] hover:text-[#EE4D2D]'>
              <BsChevronLeft size={24} />
            </button>
            <img src={chat?.avatar} alt={chat?.name} className='w-10 h-10 rounded-full object-cover' />
            <div>
              <h2 className='font-semibold text-[#333333]'>{chat?.name}</h2>
              {chat?.online && <p className='text-sm text-green-500'>Online</p>}
            </div>
          </div>
          <div className='relative'>
            <button onClick={() => setShowDropdown(!showDropdown)} className='text-[#333333] hover:text-[#EE4D2D]'>
              <BsThreeDotsVertical size={20} />
            </button>
            {showDropdown && (
              <div className='absolute right-0 mt-2 w-48 bg-white rounded-lg shadow-lg py-2 z-10'>
                <button className='w-full text-left px-4 py-2 hover:bg-gray-100'>View Profile</button>
                <button className='w-full text-left px-4 py-2 hover:bg-gray-100'>Block User</button>
                <button className='w-full text-left px-4 py-2 hover:bg-gray-100'>Report Conversation</button>
              </div>
            )}
          </div>
        </div>

        <div className='flex-1 overflow-y-auto p-4 space-y-4'>
          {dummyMessages.map((message) => (
            <div key={message.id} className={`flex ${message.sender === 'user' ? 'justify-end' : 'justify-start'}`}>
              {message.product ? (
                <div className='bg-white rounded-lg shadow p-3 max-w-sm border'>
                  <img
                    src={message.product.image}
                    alt={message.product.name}
                    className='w-full h-32 object-cover rounded-lg'
                  />
                  <h4 className='mt-2 font-semibold'>{message.product.name}</h4>
                  <p className='text-[#EE4D2D] font-semibold'>{message.product.price}</p>
                </div>
              ) : (
                <div
                  className={`max-w-[70%] rounded-lg px-4 py-2 ${
                    message.sender === 'user' ? 'bg-[#FFF0E5] text-[#333333]' : 'bg-[#F5F5F5] text-[#333333]'
                  }`}
                >
                  <p>{message.text}</p>
                  <p className='text-xs text-[#808080] mt-1'>{message.timestamp}</p>
                </div>
              )}
            </div>
          ))}
        </div>

        <MessageInput />
      </div>
    </div>
  );
};

export default ChatDetail;

const MessageInput = () => {
  const [inputMessage, setInputMessage] = useState('');

  return (
    <div className='p-4 border-t bg-white'>
      <div className='flex items-center space-x-2'>
        <button className='text-[#808080] hover:text-[#EE4D2D]'>
          <BsEmojiSmile size={20} />
        </button>
        <input
          type='text'
          placeholder='Type a message...'
          className='flex-1 rounded-full bg-gray-100 px-4 py-2 focus:outline-none focus:ring-2 focus:ring-[#EE4D2D]'
          value={inputMessage}
          onChange={(e) => setInputMessage(e.target.value)}
        />
        <button className='text-[#808080] hover:text-[#EE4D2D]'>
          <BsPaperclip size={20} />
        </button>
        <button className='bg-[#EE4D2D] text-white rounded-full p-2 hover:bg-[#ff6347]'>
          <IoMdSend size={20} />
        </button>
      </div>
    </div>
  );
};
