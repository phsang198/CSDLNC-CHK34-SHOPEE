/* eslint-disable @typescript-eslint/no-unused-vars */
import React, { useState } from 'react';
import { BsEmojiSmile, BsPaperclip, BsThreeDotsVertical, BsChevronLeft } from 'react-icons/bs';
import { IoMdSend } from 'react-icons/io';
import ChatDetail from './ChatDetails';
import { useNavigate } from 'react-router-dom';

const Chat = () => {
  const navigate = useNavigate();
  const handleClickChat = (id: number) => {
    navigate(`/chat-detail/${id}`);
  };
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

  const ChatList = () => (
    <div className='h-screen bg-white'>
      <div className='p-4 border-b'>
        <h1 className='text-xl font-semibold text-[#333333]'>Messages</h1>
      </div>
      <div className='overflow-y-auto'>
        {dummyChats.map((chat) => (
          <div
            key={chat.id}
            className='p-4 border-b hover:bg-gray-50 cursor-pointer'
            onClick={() => handleClickChat(chat.id)}
          >
            <div className='flex items-center space-x-3'>
              <div className='relative'>
                <img src={chat.avatar} alt={chat.name} className='w-12 h-12 rounded-full object-cover' />
                {chat.online && (
                  <div className='absolute bottom-0 right-0 w-3 h-3 bg-green-500 rounded-full border-2 border-white'></div>
                )}
              </div>
              <div className='flex-1'>
                <h3 className='font-semibold text-[#333333]'>{chat.name}</h3>
                <p className='text-sm text-[#808080] truncate'>{chat.lastMessage}</p>
              </div>
              <div className='text-right'>
                <p className='text-xs text-[#808080]'>{chat.timestamp}</p>
                {chat.unread > 0 && (
                  <div className='mt-1 bg-[#EE4D2D] text-white rounded-full w-5 h-5 flex items-center justify-center text-xs'>
                    {chat.unread}
                  </div>
                )}
              </div>
            </div>
          </div>
        ))}
      </div>
    </div>
  );

  return <div className='max-w-3xl mx-auto'>{<ChatList />}</div>;
};

export default Chat;
