// import { Prop, Schema, SchemaFactory } from '@nestjs/mongoose';
// import { Document } from 'mongoose';

// export type UserDocument = User & Document;

// @Schema({ versionKey: false, timestamps: true })
// export class User {
//   @Prop()
//   email: string;

//   @Prop()
//   password: string;
// }

// export const UserSchema = SchemaFactory.createForClass(User);
import { Prop, Schema, SchemaFactory } from '@nestjs/mongoose';
import { Document } from 'mongoose';

// Define the UserDocument type
export type UserDocument = User & Document;

export enum Gender {
  MALE = 'male',
  FEMALE = 'female',
  OTHER = 'other',
}

export enum Role {
  USER = 'user',
  ADMIN = 'admin',
  MODERATOR = 'moderator',
}

export enum Status {
  ACTIVE = 'active',
  INACTIVE = 'inactive',
  BANNED = 'banned',
}

@Schema({ versionKey: false, timestamps: true })
export class User {
  @Prop({ required: true, unique: true, trim: true, lowercase: true })
  email: string;

  @Prop({ required: true })
  password: string;

  @Prop({ required: false, trim: true, default: null })
  name: string | null;

  @Prop({
    required: false,
    default:
      'https://vieclam.ueh.edu.vn/images/company/avatar/KhKYCBSjQQ_Shopee-logo-512x512.png',
  })
  avatar: string | null;

  @Prop({ required: false, trim: true, default: null })
  phone: string | null;

  @Prop({ required: false, enum: Status, default: Status.ACTIVE })
  status: Status;

  @Prop({ required: false, default: null })
  birthday: Date | null;

  @Prop({ required: false, enum: Role, default: Role.USER })
  role: Role;
}

export const UserSchema = SchemaFactory.createForClass(User);
