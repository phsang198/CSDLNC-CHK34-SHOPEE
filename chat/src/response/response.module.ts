import { Module } from '@nestjs/common';
import { ResponseService } from './response.service';
import { ResponseController } from './response.controller';
import { ArangoDBModule } from 'src/arangodb';

@Module({
  imports: [ArangoDBModule],
  controllers: [ResponseController],
  providers: [ResponseService],
})
export class ResponseModule {}
